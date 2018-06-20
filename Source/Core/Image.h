#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <Platform/Platform.h>
#include "Color.h"

using namespace std;


class Image  
{
public:

	Image() 
		: mWidth( 0 ), mHeight( 0 ), mData( NULL ) {};

	Image( int width, int height ) 
		: mWidth( width ), mHeight( height )
	{
		assert( width > 0 && height > 0 );
		mData = new Color[ width * height ];
	}

	Image( int width, int height, Color initColor ) 
		: mWidth( width ), mHeight( height )
	{
		assert( width > 0 && height > 0 );
		mData = new Color[ width * height ];
		for ( int i = 0; i < width * height; i++ ) mData[i] = initColor;
	}

	~Image() { delete[] mData; }



	Image &setImage( int width, int height );

	Image &setImage( int width, int height, Color initColor );


	Image &setPixel( int x, int y, Color c ) 
	{ 
		assert( x >= 0 && x < mWidth && y >= 0 && y < mHeight ); 
		mData[ (mHeight - y - 1) * mWidth + x ] = c;
		return (*this); 
	}


	Color getPixel( int x, int y ) const
	{ 
		assert( x >= 0 && x < mWidth && y >= 0 && y < mHeight ); 
		return mData[(mHeight - y - 1) * mWidth + x];
	}

    Color* getPixel(int x, int y)
    {
        assert(x >= 0 && x < mWidth && y >= 0 && y < mHeight);
        return &mData[(mHeight - y - 1) * mWidth + x];
    }

	int width() const { return mWidth; }

	int height() const { return mHeight; }


	Image &gammaCorrect( float gamma = 2.2f );


	// Write image to a file. Returns true iff successful. 
	bool writeToFile( const char *filename ) const;
    void UpdateTexture();
    inline void* GetData() { return mData; }
    inline GLuint GetGLTextureHandle() { return mGLHandle; }
    inline uint32_t GetWidth() { return mWidth; }
    inline uint32_t GetHeight() { return mHeight; }
private:

	int mWidth, mHeight;
	Color *mData;
    GLuint mGLHandle;
	// Disallow the use of copy constructor and assignment operator.
	Image( const Image &image ) {}
    Image &operator= ( const Image &image ) {return const_cast<Image&>(image);}

}; // Image


#endif // _IMAGE_H_
