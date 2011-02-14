varying vec4 color;

varying float timeAlive;

void main ( void )
{
    gl_FragColor = color;
    
	if ( timeAlive < 0.0 || timeAlive > 0.6 )
	{
		discard;
	}
	
	if ( timeAlive < 0.1 )
	{
		gl_FragColor.y += 0.1;
	}
	else if ( timeAlive < 0.2 )
	{
		gl_FragColor.y += 0.2;
	}
	else if ( timeAlive < 0.3 )
	{
		gl_FragColor.y += 0.3;
	}
	else if ( timeAlive < 0.4 )
	{
		gl_FragColor.y += 0.4;
	}
	else if ( timeAlive < 0.5 )
	{
		gl_FragColor.y += 0.5;
	}
	else if ( timeAlive < 0.525 )
	{
		gl_FragColor = vec4( 0.4, 0.4, 0.4, 1.0 );
	}
	else if ( timeAlive < 0.550 )
	{
		gl_FragColor = vec4( 0.5, 0.5, 0.5, 1.0 );
	}
	else
	{
		gl_FragColor = vec4( 0.6, 0.6, 0.6, 1.0 );
	}
	
	clamp( gl_FragColor, 0.0, 1.0 );
}