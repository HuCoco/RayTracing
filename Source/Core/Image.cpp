#include <cstdlib>
#include <cmath>
#include <cassert>
#include "Image.h"
#include "ImageIO.h"

using namespace std;



Image &Image::setImage( int width, int height )
{
    assert( width > 0 && height > 0 );
	mWidth = width; mHeight = height;
	delete[] mData;
    mData = new Color[ width * height ];
	return (*this);
}



Image &Image::setImage( int width, int height, Color initColor )
{
	setImage( width, height );
	for ( int i = 0; i < width * height; i++ ) mData[i] = initColor;
	return (*this);
}



Image &Image::gammaCorrect( float gamma )
{
	for ( int i = 0; i < mWidth * mHeight; i++ ) 
	{
		mData[i].clamp( 0.0f, 1.0f );
		mData[i].gammaCorrect( gamma );
	}
	return (*this);
}



bool Image::writeToFile( const char *filename ) const
{
	assert( mWidth > 0 && mHeight > 0 );
	uchar *bytes = new uchar[ 4 * mWidth * mHeight ];
    int b_offset = 0;
    int g_offset = mWidth * mHeight;
    int r_offset = mWidth * mHeight * 2;
    int a_offset = mWidth * mHeight * 3;
	for ( int i = 0; i < mWidth * mHeight; i++ )
	{
		int r = (int) (256.0 * mData[i].r());
		if ( r > 255 ) r = 255;
		int g = (int) (256.0 * mData[i].g());
		if ( g > 255 ) g = 255;
		int b = (int) (256.0 * mData[i].b());
		if ( b > 255 ) b = 255;


		bytes[4*i + 0] = (uchar)b;
		bytes[4*i + 1] = (uchar)g;
		bytes[4*i + 2] = (uchar)r;
        bytes[4*i + 3] = (uchar)255;
	}

	int status = ImageIO::SaveImageFile( filename, bytes, mWidth, mHeight, 4 );

	delete[] bytes;
	return ( status == 1 );
}

void Image::CreateTextureData(void* data)
{
    uchar* pData = (uchar*)data;
    for (int i = 0; i < mWidth * mHeight; i++)
    {
        int r = (int)(256.0 * mData[i].r());
        if (r > 255) r = 255;
        int g = (int)(256.0 * mData[i].g());
        if (g > 255) g = 255;
        int b = (int)(256.0 * mData[i].b());
        if (b > 255) b = 255;


        pData[4 * i + 0] = (uchar)r;
        pData[4 * i + 1] = (uchar)g;
        pData[4 * i + 2] = (uchar)b;
        pData[4 * i + 3] = (uchar)255;
    }
}
