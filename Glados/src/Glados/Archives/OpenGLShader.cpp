#include "gladospch.h"
#include "OpenGLShader.h"
#include "Object.h"

namespace Glados {

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_Filepath(filepath), m_RendererID(0)
    {
        ShaderProgramSource source = ParseShader(filepath);
        m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    }

    OpenGLShader::~OpenGLShader()
    {
        GLCall(glDeleteProgram(m_RendererID));
    }

    ShaderProgramSource OpenGLShader::ParseShader(const std::string& filepath)
    {
        std::ifstream stream(filepath);

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else
            {
                ss[int(type)] << line << "\n";
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    unsigned int OpenGLShader::CompileShader(unsigned int type, const std::string& source)
    {
        // create an empty shader object to hold shader strings
        GLCall(unsigned int id = glCreateShader(type));
        const char* src = source.c_str();
        // add source code for shader
        GLCall(glShaderSource(id, 1, &src, nullptr));
        // compile the shader
        GLCall(glCompileShader(id));

        // if the shader compiled or not
        int result;
        GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
        if (result == GL_FALSE)
        {
            // get number of characters in the info log
            int length;
            GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
            // allocate memory on the stack for chars
            char* message = (char*)_malloca(length * sizeof(char));
            // assigns the message to be called
            GLCall(glGetShaderInfoLog(id, length, &length, message));
            std::cout << "Failed to comple " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                << std::endl;
            std::cout << message << std::endl;
            GLCall(glDeleteShader(id));
            return 0;
        }

        return id;
    }

    int OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        GLCall(unsigned int program = glCreateProgram());
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        // attaches shaders to the current program
        GLCall(glAttachShader(program, vs));
        GLCall(glAttachShader(program, fs));
        GLCall(glLinkProgram(program));
        GLCall(glValidateProgram(program));

        // deletes used shaders
        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));

        return program;
    }

    void OpenGLShader::Bind() const
    {
        GLCall(glUseProgram(m_RendererID));
    }

    void OpenGLShader::Unbind() const
    {
        GLCall(glUseProgram(0));
    }

    // sets the binding point of a uniform block and returns its index
    void OpenGLShader::SetUniformBlockIndex(const std::string& name, unsigned int binding)
    {
        GLCall(unsigned int ubo = glGetUniformBlockIndex(m_RendererID, name.c_str()));
        GLCall(glUniformBlockBinding(m_RendererID, ubo, binding));
    }

    void OpenGLShader::SetUniform4f(const std::string& name,
        float v0, float v1, float v2, float v3)
    {
        GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void OpenGLShader::SetUniform4fv(const std::string& name, const float* value)
    {
        GLCall(glUniform4fv(GetUniformLocation(name), 1, value));
    }

    void OpenGLShader::SetUniform3f(const std::string& name,
        float v0, float v1, float v2)
    {
        GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
    }

    void OpenGLShader::SetUniform3fv(const std::string& name, const float* value)
    {
        GLCall(glUniform3fv(GetUniformLocation(name), 1, value));
    }

    void OpenGLShader::SetUniformMat4fv(const std::string& name, bool transpose, const float* value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, value);
    }

    void OpenGLShader::SetUniform1f(const std::string& name, float f)
    {
        GLCall(glUniform1f(GetUniformLocation(name), f));
    }

    void OpenGLShader::SetUniform1i(const std::string& name, int i)
    {
        GLCall(glUniform1i(GetUniformLocation(name), i));
    }

    void OpenGLShader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
    {
        GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }

    int OpenGLShader::GetUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
        if (location == -1)
        {
            std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
            m_UniformLocationCache[name] = location;
        }
        //std::cout << location << std::endl;
        return location;
    }

#ifdef _DEBUG
    void OpenGLShader::PrintUniforms()
    {
        GLPrintUniformInfo(m_RendererID);
    }
#else
    void OpenGLShader::PrintUniforms() {}
#endif

}