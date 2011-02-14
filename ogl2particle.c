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

//
// The source of the shaders itself are in plain text files. Files ending in
// .vert are vertex shaders, and files ending in .frag are fragment shaders.
//
#define GL_GLEXT_PROTOTYPES
#include "ogl2particle.h"

//
// flags for doing animation
//
float ParticleTime;

//
// Handle of the window we're rendering to
//
static GLint window;

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }

    return retCode;
}



/******************************************************************************/
/*
/* GLUT glue
/*
/******************************************************************************/

int Args(int argc, char **argv)
{
    int i;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-loop") == 0)
        {
            if (i+1 >= argc || argv[i+1][0] == '-')
            {
                printf("-loop no count given.\n");
                return FALSE;
            }
        }
        else
        {
            printf("%s (Bad option)\n", argv[i]);
            return FALSE;
        }
    }
    return TRUE;
}


void display(void)
{
    glLoadIdentity();

	glScalef( 0.25f, 0.25f, 0.25f );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawPoints();

    glFlush();
    glutSwapBuffers();
}


void updateAnim()
{
    int location;

    location = getUniLoc(ProgramObject, "currentTime"); 
    ParticleTime += 0.002f;

    if (ParticleTime > 15.0)
        ParticleTime = 0.0;

    glUniform1fARB(location, ParticleTime);

    printOpenGLError();
}


void play(void)
{
    updateAnim();

    printOpenGLError();
    glutPostRedisplay();
    printOpenGLError();
}


void key( unsigned char keyPressed, int x, int y )
{
    switch( keyPressed )
    {
        case 'q':
            exit( EXIT_SUCCESS );
            break;

        default:
            break;
    }
}


void reshape(int wid, int ht)
{
    float vp = 0.8f;
    float aspect = (float) wid / (float) ht;

    glViewport(0, 0, wid, ht);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, wid, ht);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
    glFrustum(-vp, vp, -vp / aspect, vp / aspect, 3, 10.0);

	gluLookAt( 3.0f, // eye
               3.0f,
               3.0f,
               0.0f, // center
               0.0f,
               0.0f,
               0.0f, // up
               1.0f,
               0.0f );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}


// The recommended technique for querying OpenGL extensions;
// from http://opengl.org/resources/features/OGLextensions/

int isExtensionSupported( const char *extension )
{
    const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;

    /* Extension names should not have spaces. */
    where = (GLubyte *) strchr(extension, ' ');
    if (where || *extension == '\0')
        return 0;

    extensions = glGetString(GL_EXTENSIONS);

    /* It takes a bit of care to be fool-proof about parsing the
    OpenGL extensions string. Don't be fooled by sub-strings, etc. */
    start = extensions;
    for (;;) {
        where = (GLubyte *) strstr((const char *) start, extension);
        if (!where)
            break;
        terminator = where + strlen(extension);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return 1;
        start = terminator;
    }
    return 0;
}


/******************************************************************************/
/*
/* Main
/*
/******************************************************************************/

#define PADDR(functype,funcname) \
    ((funcname = (functype) wglGetProcAddress( #funcname )) == NULL)

int main( int argc, char **argv )
{
    int success = 0;
    GLcharARB *VertexShaderSource, *FragmentShaderSource;

    int error = 0;

    if (Args(argc, argv) == FALSE)
        exit(0);

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize(500, 500);
    window = glutCreateWindow( "Fire and Smoke" );

    glutIdleFunc( play );
    glutDisplayFunc( display );
    glutKeyboardFunc( key );
    glutReshapeFunc( reshape );

    printOpenGLError();

    if ( !isExtensionSupported( "GL_ARB_shader_objects" )   ||
         !isExtensionSupported( "GL_ARB_fragment_shader" )  ||
         !isExtensionSupported( "GL_ARB_vertex_shader" )    ||
         !isExtensionSupported( "GL_ARB_shading_language_100" ) )
    {
        printf("OpenGL Shading Language extensions not available\n" );
        return 1;
    }

    if( error )
    {
        printf( "ERROR from wglGetProcAddress\n" );
    }

    createPoints( numParticles );

    glDepthFunc( GL_LESS );
    glEnable( GL_DEPTH_TEST );
	glClearColor( 0.0f, 0.67f, 0.94f, 1.0f );

	ParticleTime = 0.0f;

    readShaderSource( "smoke", &VertexShaderSource, &FragmentShaderSource );
    success = installParticleShaders( VertexShaderSource, FragmentShaderSource );

    if ( success )
	{
		glutMainLoop();
	}
	else
	{
		printf( "Error installing particle shaders" );
		scanf( "%d", ParticleTime );
	}

    return 0;
}
