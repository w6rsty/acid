/// TODO: Add uniform location cache
/// TODO: Add uniform auto read
/// TODO: Change assertion way to support runtime error handling

#include "platform/OpenGL/opengl_shader.hpp"

#include "core/assert.hpp"

#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <array>
#include <vector>

namespace acid
{

static GLenum ShaderTypeStrToGLeum(const std::string& type)
{
    if (type == "vertex") return GL_VERTEX_SHADER;
    else if (type == "fragment") return GL_FRAGMENT_SHADER;
    else {
        // Parsing function will do the assertion
        AC_TAG_WARN("Shader", "Unsupport shader type: {", type, "}");
        return 0;
    }
}

static std::string ShaderTypeGLeumToStr(const GLenum type)
{
    if (type == GL_VERTEX_SHADER) return "vertex";
    else if (type == GL_FRAGMENT_SHADER) return "fragment";
    else {
        AC_TAG_WARN("Shader", "Unsupport shader type: {", type, "}");
        return "";
    }
}

OpenGLShader::OpenGLShader(const std::string& filename)
{
    std::string content = ReadFile(filename);
    auto shaderSources = PreProcess(content);
    CompileShader(shaderSources);
}

OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    std::unordered_map<GLenum, std::string> shaderSources;
    shaderSources[GL_VERTEX_SHADER] = vertexSrc;
    shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
    CompileShader(shaderSources);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(rendererID_);
}

void OpenGLShader::Bind() const
{
    glUseProgram(rendererID_);
}

void OpenGLShader::Unbind() const
{
    glUseProgram(0);
}

std::string OpenGLShader::ReadFile(const std::string& filename)
{
   std::ifstream file(filename, std::ios::in | std::ios::binary);
   if (file.fail()) {
        AC_LOG_ERROR("Failed to open file: ", filename);
        return "";
   }

    std::string content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());
    file.close();

    return content;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& content)
{
    std::unordered_map<GLenum, std::string> shaderSources;

    static std::string typeToken = "#type";
    static size_t typeTokenLength = typeToken.length();

    size_t pos = content.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        size_t eol = content.find_first_of("\r\n", pos);
        AC_ASSERT_MSG(eol != std::string::npos, "Sytanx error!");
        size_t begin = pos + typeTokenLength + 1;
        std::string typeName = content.substr(begin, eol - begin);
        // strip spaces
        typeName.erase(std::remove_if(typeName.begin(), typeName.end(), ::isspace), typeName.end());
        AC_LOG_DEBUG("Reading shader type: {",typeName,"}");
        GLenum type = ShaderTypeStrToGLeum(typeName);
        AC_ASSERT_BLANK(type != 0, "Invalid shader type");
        size_t sourceBegin = content.find_first_not_of("\r\n", eol);
        pos = content.find(typeToken, sourceBegin);
        std::string source = content.substr(sourceBegin, pos - (sourceBegin == std::string::npos ? content.size() - 1 : sourceBegin));
        AC_ASSERT_MSG(!source.empty(), "Invalid shader source");
        shaderSources[type] = source;
    }
    return shaderSources;
}

void OpenGLShader::CompileShader(const std::unordered_map<GLenum, std::string>& shaderSources)
{
    uint32_t program = glCreateProgram();
    std::array<GLenum, 2> shaderIDs;
    uint32_t shaderIndex = 0;
    // ts stands for type and source
    for (auto& ts : shaderSources)
    {
        GLenum type = ts.first;
        const char* sourceCStr = ts.second.c_str();

        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &sourceCStr, NULL);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

            glDeleteShader(shader);
            glDeleteProgram(program);
            AC_ASSERT_MSG(false, ShaderTypeGLeumToStr(ts.first), " shader compilation failed: \n", infoLog.data());
        }
        glAttachShader(program, shader);
        shaderIDs[shaderIndex++] = shader;
    }

    glLinkProgram(program);
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteProgram(program);
        for (auto id : shaderIDs) {
            glDeleteShader(id);
        }

        AC_ASSERT_MSG(false, "Shader link failed: \n", infoLog.data());
        return;
    }
    for (auto id : shaderIDs) {
        glDetachShader(program, id);
    }

    rendererID_ = program;
}

void OpenGLShader::SetUniformInt(const std::string &name, const int value)
{
    UploadUniformFloat(name, value);
}

void OpenGLShader::SetUniformIntArray(const std::string &name, int* values, uint32_t count)
{
    UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetUniformFloat(const std::string &name, const float value)
{
    UploadUniformFloat(name, value);
}

void OpenGLShader::SetUniformFloat2(const std::string &name, const glm::vec2& value)
{
    UploadUniformFloat2(name, value);
}

void OpenGLShader::SetUniformFloat3(const std::string &name, const glm::vec3& value)
{
    UploadUniformFloat3(name, value);
}

void OpenGLShader::SetUniformFloat4(const std::string &name, const glm::vec4& value)
{
    UploadUniformFloat4(name, value);
}

void OpenGLShader::SetUniformMat3(const std::string &name, const glm::mat3& value)
{
    UploadUniformMat3(name, value);
}

void OpenGLShader::SetUniformMat4(const std::string &name, const glm::mat4& value)
{
    UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
{
    GLint location = glGetUniformLocation(rendererID_, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
{
    GLint location = glGetUniformLocation(rendererID_, name.c_str());
    glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
{
    GLint location = glGetUniformLocation(rendererID_, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
{
    GLint location = glGetUniformLocation(rendererID_, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(rendererID_, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
{
    GLint location = glGetUniformLocation(rendererID_, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
{
    GLint location = glGetUniformLocation(rendererID_, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
{
    GLint location = glGetUniformLocation(rendererID_, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::BindUniformBlock(const std::string &name, uint32_t index)
{
    GLuint blockIndex = glGetUniformBlockIndex(rendererID_, name.c_str());
    glUniformBlockBinding(rendererID_, blockIndex, index);
}

} // namespace acid