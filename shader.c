/************************************************************************
 *                                                                      *
 *              Copyright (C) 2002-2004  3Dlabs Inc. Ltd.               *
 *                                                                      *
 *                        All rights reserved.                          *
 *                                                                      *
 * Redistribution and use in source and binary forms, with or without   *
 * modification, are permitted provided that the following conditions   *
 * are met:                                                             *
 *                                                                      *
 *     Redistributions of source code must retain the above copyright   *
 *     notice, this list of conditions and the following disclaimer.    *
 *                                                                      *
 *     Redistributions in binary form must reproduce the above          *
 *     copyright notice, this list of conditions and the following      *
 *     disclaimer in the documentation and/or other materials provided  *
 *     with the distribution.                                           *
 *                                                                      *
 *     Neither the name of the 3Dlabs nor the names of its              *
 *     contributors may be used to endorse or promote products derived  *
 *     from this software without specific prior written permission.    *
 *                                                                      *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS  *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT    *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS    *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE       *
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,  *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, *
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;     *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER     *
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT   *
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN    *
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
 * POSSIBILITY OF SUCH DAMAGE.                                          *
 *                                                                      *
/************************************************************************/

#include "ogl2particle.h"

//
// OpenGL Shading Language entry points
//
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB;

PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;

PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB;

PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB;

PFNGLUNIFORM1FARBPROC glUniform1fARB;
PFNGLUNIFORM2FARBPROC glUniform2fARB;
PFNGLUNIFORM3FARBPROC glUniform3fARB;
PFNGLUNIFORM4FARBPROC glUniform4fARB;

PFNGLUNIFORM1IARBPROC glUniform1iARB;
PFNGLUNIFORM2IARBPROC glUniform2iARB;
PFNGLUNIFORM3IARBPROC glUniform3iARB;
PFNGLUNIFORM4IARBPROC glUniform4iARB;

PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB;

PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
PFNGLUNIFORM4IVARBPROC glUniform4ivARB;

PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;

PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;

PFNGLTEXIMAGE3DPROC glTexImage3D;
PFNGLACTIVETEXTUREPROC glActiveTexture;


//
// Global handles for the currently active program object, with its two shader objects
//
GLhandleARB ProgramObject = 0;
GLhandleARB VertexShaderObject = 0;
GLhandleARB FragmentShaderObject = 0;

//
// Get the location of a uniform variable
//
GLint getUniLoc(GLhandleARB program, const GLcharARB *name)
{
    GLint loc;

    loc = glGetUniformLocationARB(program, name);

    if (loc == -1)
        printf("No such uniform named \"%s\"\n", name);

    printOpenGLError();
    return loc;
}

//
// Print out the information log for a shader object or a program object
//
void printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    GLcharARB *infoLog;

    printOpenGLError();

    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                         &infologLength);
    printOpenGLError();

    if (infologLength > 0)
    {
        infoLog = (GLcharARB*)malloc(infologLength);
        if (infoLog == NULL)
        {
            printf("ERROR: Could not allocate InfoLog buffer\n");
            exit(1);
        }
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
        printf("InfoLog:\n%s\n\n", infoLog);
        free(infoLog);
    }
    printOpenGLError();
}


int shaderSize(char *fileName, EShaderType shaderType)
{
    //
    // Returns the size in bytes of the shader fileName.
    // If an error occurred, it returns -1.
    //
    // File name convention:
    //
    // <fileName>.vert
    // <fileName>.frag
    //
    int shader;
    char name[100];
    int count;

    strcpy(name, fileName);

    switch (shaderType)
    {
        case EVertexShader:
            strcat(name, ".vert");
            break;
        case EFragmentShader:
            strcat(name, ".frag");
            break;
        default:
            _ASSERT(0);
            break;
    }

    //
    // Open the file
    //
    shader = _open(name, _O_RDONLY);
    if (shader == -1)
        return -1;

    //
    // Seek to the end and find its position
    //
    count = _lseek(shader, 0, SEEK_END);

    _close(shader);
    return count;
}


int readShader(char *fileName, EShaderType shaderType, char *shaderText, int size)
{
    //
    // Reads a shader from the supplied file and returns the shader in the
    // arrays passed in. Returns 1 if successful, 0 if an error occurred.
    // The parameter size is an upper limit of the amount of bytes to read.
    // It is ok for it to be too big.
    //
    FILE *shader;
    char name[100];
    int count;

    strcpy(name, fileName);

    switch (shaderType) 
    {
        case EVertexShader:
            strcat(name, ".vert");
            break;
        case EFragmentShader:
            strcat(name, ".frag");
            break;
        default:
            _ASSERT(0);
            break;
    }

    //
    // Open the file
    //
    shader = fopen(name, "r");
    if (!shader)
        return -1;

    //
    // Get the shader from a file.
    //
    fseek(shader, 0, SEEK_SET);
    count = fread(shaderText, 1, size, shader);
    shaderText[count] = '\0';

    if (ferror(shader))
        count = 0;
    else count = 1;

    fclose(shader);
    return count;
}


int readShaderSource(char *fileName, GLcharARB **vertexShader, GLcharARB **fragmentShader)
{
    int vSize, fSize;

    //
    // Allocate memory to hold the source of our shaders.
    //
    vSize = shaderSize(fileName, EVertexShader);
    fSize = shaderSize(fileName, EFragmentShader);

    if ((vSize == -1) || (fSize == -1))
    {
        printf("Cannot determine size of the shader %s\n", fileName);
        return 0;
    }

    *vertexShader = (GLcharARB *) malloc(vSize);
    *fragmentShader = (GLcharARB *) malloc(fSize);

    //
    // Read the source code
    //
    if (!readShader(fileName, EVertexShader, *vertexShader, vSize))
    {
        printf("Cannot read the file %s.vert\n", fileName);
        return 0;
    }

    if (!readShader(fileName, EFragmentShader, *fragmentShader, fSize)) {
        printf("Cannot read the file %s.frag\n", fileName);
        return 0;
    }

    return 1;
}


int installParticleShaders(GLcharARB *particleVertex,
                           GLcharARB *particleFragment)
{
    GLint       vertCompiled, fragCompiled;    // status values
    GLint       linked;

    // Create a vertex shader object and a fragment shader object

    VertexShaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    FragmentShaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    // Load source code strings into shaders

    glShaderSourceARB(VertexShaderObject, 1, &particleVertex, NULL);
    glShaderSourceARB(FragmentShaderObject, 1, &particleFragment, NULL);

    // Compile the particle vertex shader, and print out
    // the compiler log file.

    glCompileShaderARB(VertexShaderObject);
    printOpenGLError();
    glGetObjectParameterivARB(VertexShaderObject,
                GL_OBJECT_COMPILE_STATUS_ARB, &vertCompiled);
    printInfoLog(VertexShaderObject);

    // Compile the particle vertex shader, and print out
    // the compiler log file.

    glCompileShaderARB(FragmentShaderObject);
    printOpenGLError();
    glGetObjectParameterivARB(FragmentShaderObject,
                GL_OBJECT_COMPILE_STATUS_ARB, &fragCompiled);
    printInfoLog(FragmentShaderObject);

    if (!vertCompiled || !fragCompiled)
        return 0;

    // Create a program object and attach the two compiled shaders

    ProgramObject = glCreateProgramObjectARB();
    glAttachObjectARB(ProgramObject, VertexShaderObject);
    glAttachObjectARB(ProgramObject, FragmentShaderObject);

    // Bind generic attribute indices to attribute variable names

    glBindAttribLocationARB( ProgramObject, VELOCITY_ARRAY_INDEX, "velocity" );
    glBindAttribLocationARB( ProgramObject, START_TIME_ARRAY_INDEX, "startTime" );
	glBindAttribLocationARB( ProgramObject, RANDOM_ARRAY_INDEX, "random" );

    // Link the program object and print out the info log

    glLinkProgramARB(ProgramObject);
    printOpenGLError();
    glGetObjectParameterivARB(ProgramObject,
                GL_OBJECT_LINK_STATUS_ARB, &linked);
    printInfoLog(ProgramObject);

    if (!linked)
        return 0;

    // Install program object as part of current state

    glUseProgramObjectARB(ProgramObject);

    // Set up initial uniform values

    glUniform1fARB(getUniLoc(ProgramObject, "currentTime"), -5.0);
    printOpenGLError();

    return 1;
}
