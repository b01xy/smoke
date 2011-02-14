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

#ifndef __ogl2_particle_h__
#define __ogl2_particle_h__

#include <windows.h>
#include <winuser.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <GL/glu.h>
#include "glut.h"
#include <crtdbg.h>

#include "glext.h"

////////////////
// Structures //
////////////////

//
// Shader types
//
typedef enum {
    EVertexShader,
    EFragmentShader,
} EShaderType;


//
// Constants
//
#define VELOCITY_ARRAY_INDEX 3
#define START_TIME_ARRAY_INDEX 4
#define RANDOM_ARRAY_INDEX 5

static const int numParticles = 50000;
static const float emitterRadius = 0.4f;


//
// OpenGL Shading Language entry points
//

extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
extern PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;

extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;

extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;

extern PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB;

extern PFNGLUNIFORM1FARBPROC glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC glUniform4fARB;

extern PFNGLUNIFORM1IARBPROC glUniform1iARB;
extern PFNGLUNIFORM2IARBPROC glUniform2iARB;
extern PFNGLUNIFORM3IARBPROC glUniform3iARB;
extern PFNGLUNIFORM4IARBPROC glUniform4iARB;

extern PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC glUniform4fvARB;

extern PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
extern PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
extern PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
extern PFNGLUNIFORM4IVARBPROC glUniform4ivARB;

extern PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
extern PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
extern PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
extern PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
extern PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
extern PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
extern PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
extern PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;

extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;

extern PFNGLTEXIMAGE3DPROC glTexImage3D;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;


// 
// Objects
//
extern GLhandleARB ProgramObject;
extern GLhandleARB VertexShaderObject;
extern GLhandleARB FragmentShaderObject;

////////////////
// Prototypes //
////////////////

void drawPoints(void);
void createPoints( int numParticles );

int printOglError(char *file, int line);
#define printOpenGLError() printOglError(__FILE__, __LINE__)

void printInfoLog(GLhandleARB object);
GLint getUniLoc(GLhandleARB progObject, const GLcharARB *name);

int readShaderSource(char *fileName, GLcharARB **vertexShader, GLcharARB **fragmentShader);
int installParticleShaders(GLcharARB *particleVertex, GLcharARB *particleFragment);

#endif // __ogl2_particle_h__
