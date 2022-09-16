#include "PostProcessor.h"

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)

PostProcessor::PostProcessor(Shader& shader, unsigned int width, unsigned int height) : 
	shader(shader), Width(width), Height(height), Chaos(false), Confuse(false), Shake(false)
{
    glGenFramebuffers(1, &this->multiSampledFBO);
    glGenFramebuffers(1, &this->FBO);
    glGenRenderbuffers(1, &this->RBO);
    // Multi-sampled Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->multiSampledFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); 
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Normal frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->texture.Generate(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0); 
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Init relevent data and shader's uniforms
    this->initRenderData();
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right    
    };
    glUniform2fv(glGetUniformLocation(this->shader.ID, "offsets"), 9, (float*)offsets);
    int edgeKernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(this->shader.ID, "edgeKernel"), 9, edgeKernel);
    float blurKernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(this->shader.ID, "blurKernel"), 9, blurKernel);
}

void PostProcessor::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->multiSampledFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->multiSampledFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::Render(float time)
{
    this->shader.Use();
    this->shader.setFloat("time", time);
    this->shader.setBool("chaos", this->Chaos);
    this->shader.setBool("confuse", this->Confuse);
    this->shader.setBool("shake", this->Shake);
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    this->texture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{

    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
