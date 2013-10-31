#include "slox/SloxTextureFactory.hpp"

namespace slox {

static std::string m_message;

const std::string& SloxTextureFactory::getMessage() {
    return m_message;
}

int SloxTextureFactory::readBitmapFile( const char* filename, unsigned int* texture_r ) {
    int ret = 0;
    unsigned int texture;
    SDL_Surface* surface;
    int nOfColors;
    GLenum texture_format;

    if ( (surface = SDL_LoadBMP( filename )) ) { 
    
	    // Check that the image's width is a power of 2
	    if ( (surface->w & (surface->w - 1)) != 0 ) {
            m_message = "Warning: width not power of 2"       ;
            ret = 1;
	    }
    
	    // Also check if the height is a power of 2
	    if ( (surface->h & (surface->h - 1)) != 0 ) {
            m_message = "Warning: height not power of 2";
            ret = 1;
	    }
    
            // get the number of channels in the SDL surface
            nOfColors = surface->format->BytesPerPixel;
            if (nOfColors == 4)     // contains an alpha channel
            {
                    if (surface->format->Rmask == 0x000000ff)
                            texture_format = GL_RGBA;
                    else
                            texture_format = GL_BGRA;
            } else if (nOfColors == 3)     // no alpha channel
            {
                    if (surface->format->Rmask == 0x000000ff)
                            texture_format = GL_RGB;
                    else
                            texture_format = GL_BGR;
            } else {
                    m_message = "warning: image not truecolor\n";
                    // this error should not go unhandled
            }
    
	    // Have OpenGL generate a texture object handle for us
	    glGenTextures( 1, &texture );
    
	    // Bind the texture object
	    glBindTexture( GL_TEXTURE_2D, texture );
    
	    // Set the texture's stretching properties
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
	    // Edit the texture object's image data using the information SDL_Surface gives us
	    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                        texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    } 
    else {
        char error[ 256 ];
        snprintf( error, 256, "Error: could not load image: %s\n", SDL_GetError() );
        m_message = error;
	    return -1;
    }    
    
    // Free the SDL_Surface only if it was successfully created
    if ( surface ) { 
	    SDL_FreeSurface( surface );
    }

    *texture_r = texture;
    return ret;
}

}