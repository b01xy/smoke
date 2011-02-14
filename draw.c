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

static GLfloat* vertices = NULL;
static GLfloat* colors = NULL;
static GLfloat* velocities = NULL;
static GLfloat* startTimes = NULL;
static GLfloat* myrandoms = NULL;



float myrandom( float low, float high )
{
	float range = high - low;
	float multiplier = ( float ) rand() / RAND_MAX;

	return range * multiplier + low;
}

float fireRed()
{
	return 1.0 - myrandom( 0.0f, 0.2f );
}

float fireGreen()
{
	return myrandom( 0.0f, 0.5f );
}

float fireBlue()
{
	return 0.0f;
}

void fillRandom( int numPoints )
{
	GLfloat* myrandomPointer;
	int counter;

	if ( myrandoms == NULL )
	{
		myrandoms = malloc( numPoints * 3 * sizeof( float ) );
	}

	myrandomPointer = myrandoms;

	for ( counter = 0; counter < numPoints * 3; counter++ )
	{
		*myrandomPointer = myrandom( -1.0f, 1.0f );
		myrandomPointer++;
	}
}

void createPoints( int numPoints )
{
	GLfloat* vertexPointer;
	GLfloat* colorPointer;
	GLfloat* velocityPointer;
	GLfloat* startTimePointer;

	float newRadius;
	float angle;

	GLint counter;

	if ( vertices != NULL )
	{
		free( vertices );
	}


	vertices  = malloc( numPoints * 3 * sizeof( float ) );
	colors = malloc( numPoints * 3 * sizeof( float ) );
	velocities = malloc( numPoints * 3 * sizeof( float ) );
	startTimes = malloc( numPoints * sizeof( float ) );

	vertexPointer = vertices;
	colorPointer = colors;
	velocityPointer = velocities;
	startTimePointer  = startTimes;

	for ( counter = 0; counter < numPoints; counter++ )
	{
		newRadius = myrandom( 0.0f, 1.0f ) * emitterRadius;
		angle = myrandom( 0.0f, 360.0f );

		*vertexPointer = 0.0f;
		*( vertexPointer + 1 ) = newRadius * sin( angle );
		*( vertexPointer + 2 ) = newRadius * cos( angle );
		vertexPointer += 3;

		*colorPointer = fireRed();
		*( colorPointer + 1 ) = fireGreen();
		*( colorPointer + 2 ) = fireBlue();
		colorPointer += 3;

		*velocityPointer = myrandom( 2.0f, 10.0f );
		*( velocityPointer + 1 ) = myrandom( 0.5f, 1.0f );
		*( velocityPointer + 2 ) = myrandom( 0.5f, 1.0f );
		velocityPointer += 3;

		*startTimePointer = myrandom( 0.0f, 10.0f );
		startTimePointer++;
	}

	fillRandom( numPoints );
}

void drawPoints()
{	
	fillRandom( numParticles );

	glPointSize( 2.0 );

	glVertexPointer( 3, GL_FLOAT, 0, vertices );
	glColorPointer( 3, GL_FLOAT, 0, colors );
	glVertexAttribPointerARB( VELOCITY_ARRAY_INDEX,  3, GL_FLOAT, GL_FALSE, 0, velocities );
	glVertexAttribPointerARB( START_TIME_ARRAY_INDEX, 1, GL_FLOAT, GL_FALSE, 0, startTimes );
	glVertexAttribPointerARB( RANDOM_ARRAY_INDEX, 3, GL_FLOAT, GL_FALSE, 0, myrandoms );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableVertexAttribArrayARB( VELOCITY_ARRAY_INDEX );
	glEnableVertexAttribArrayARB( START_TIME_ARRAY_INDEX );
	glEnableVertexAttribArrayARB( RANDOM_ARRAY_INDEX );

	glDrawArrays( GL_POINTS, 0, numParticles );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableVertexAttribArrayARB( VELOCITY_ARRAY_INDEX );
	glDisableVertexAttribArrayARB( START_TIME_ARRAY_INDEX );
	glDisableVertexAttribArrayARB( RANDOM_ARRAY_INDEX );
}
