#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>    // std::copy

#include "LedTexture.h"
#include "LedColor.h"
#include "LedColors.h"

// Empty constructor
LedTexture::LedTexture()
{
	mWidth = 0;
	mHeight = 0;
	mPixels = nullptr;
}

// Constructor for definining an empty texture by explicit dimensions
LedTexture::LedTexture(int width, int height)
{
	mWidth = width;
	mHeight = height;
	mPixels = new LedColor[mWidth * mHeight];
}

// Constructor for taking over an existing source of pixel
LedTexture::LedTexture(int width, int height, LedColor *source, bool doCopy)
{
	mWidth = width;
	mHeight = height;
	
	if (!doCopy)
		mPixels = source;
	else
	{
		int num = mWidth * mHeight;
		mPixels = new LedColor[num];
		std::copy_n(source, num, mPixels);
	}
}

LedTexture::LedTexture(std::string fn)
{
	LoadBmpInternal(fn);
}

// clone
LedTexture& LedTexture::Clone()
{
	LedTexture res(mWidth, mHeight, mPixels, true);
	return res;
}

// save
LedColor LedTexture::Get(int x, int y)
{
	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
		return LedColors.Black;
	return mPixels[mWidth * y + x];
}

// save
void LedTexture::Put(int x, int y, LedColor pixel)
{
	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
		return;
	mPixels[mWidth * y + x] = pixel;
}

// unsafe!!
LedColor* LedTexture::operator[](int row)
{
	return &mPixels[mWidth * row];
}

//
//
// ----------------- Drawing --------------------
//
//

void LedTexture::FillRect(LedColor color, int x, int y, int width, int height)
{
	// clip start X, Y
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= mWidth || y >= mHeight)
		return;

	// establish width and height
	if (width < 0)
		width = mWidth;
	if (height < 0)
		width = mHeight;

	if (x + width >= mWidth)
		width = mWidth - x;
	if (y + height >= mHeight)
		height = mHeight - y;

	// finally do the intended work
	for (int iy = y; iy < y + height; y++)
		for (int ix = x; ix < x + width; x++)
			Put(ix, iy, color);
}

//
//
// ----------------- Gradient --------------------
//
//

void LedTexture::FillGradient(LedGradient gradient, double f)
{
	for (int y = 0; y < mHeight; y++)
		for (int x = 0; x < mWidth; x++)
		{
			double shade = 0.0;

			switch (gradient)
			{
				case LeftToRight:
				{
					double midX = 1.0 * mWidth * f;
					shade = std::max(0.0, std::min(1.0, 0.1 * (1.0 * x - midX)));
					break;
				}
				case RightToLeft:
				{
					double midX = 1.0 * mWidth - 1.0 * mWidth * f;
					shade = std::max(0.0, std::min(1.0, 0.1 * (1.0 * x - midX)));
					break;
				}
				case TopToBottom:
				{
					double midY = 1.0 * mHeight * f;
					shade = std::max(0.0, std::min(1.0, 0.1 * (1.0 * y - midY)));
					break;
				}
				case TopLeftToRightBottom:
				{
					double aspect = 1 * mHeight / mWidth;
					double midX = (-1.5 * aspect * mWidth) + 3.3 * aspect * mWidth * f;
					shade = std::max(0.0, std::min(1.0,
						0.05 * ((1.0 * y - 64.0) + (1.0 * x - midX))
					));
					break;
				}
				default:
					break;
			}

			Put(x, y, LedColors.White * shade);
		}
}

//
//
// ----------------- Blitting --------------------
//
//

bool LedTexture::BlitFrom(
	int destX, int destY,
	LedTexture& src,
	int srcX, int srcY,
	int srcWidth, int srcHeight)
{
	// first check
	if (destX < 0 || destX >= mWidth || destY < 0 || destY >= mHeight)
		return false;

	// establish width and height
	int sw = src.mWidth;
	if (srcWidth >= 0)
		sw = srcWidth;

	int sh = src.mHeight;
	if (srcHeight >= 0)
		sh = srcHeight;

	// now, do nested loop and break if it pointless
	for (int y = 0; y < sh; y++)
	{
		// source/ dest X,Y
		int sy = srcY + y;
		int dy = destY + y;

		// pointless?
		if (sy >= src.mHeight || dy >= mHeight)
			break;

		// same for x
		for (int x = 0; x < sw; x++)
		{
			// source/ dest X,Y
			int sx = srcX + x;
			int dx = destX + x;
			
			if (sx >= 0 && sx < src.mWidth && sy >= 0 && sy < src.mHeight
				&& dx >= 0 && dx < mWidth && dy >= 0 && dy < mHeight)
			{
				// unsafe is most efficient and ok in these guards
				(*this)[dy][dx] = src[sy][sx];
			}
		}
	}

	// OK
	return true;
}

bool LedTexture::BlendFrom(
	LedTexture& source1,
	LedTexture& source2,
	LedTexture& gradient,
	LedBlendEffect effect)
{
	// check for validity
	if (   !HasContent() || !source1.HasContent() 
		|| !source2.HasContent() || !gradient.HasContent()
		|| source1.GetWidth() != GetWidth()
		|| source1.GetWidth() != source2.GetWidth()
		|| source1.GetWidth() != gradient.GetWidth()
		|| source1.GetHeight() != GetHeight()
		|| source1.GetHeight() != source2.GetHeight()
		|| source1.GetHeight() != gradient.GetHeight())
	{
		return false;
	}

	// per pixel
	for (int y=0; y < mHeight; y++)
		for (int x = 0; x < mWidth; x++)
		{
			double grad = gradient.Get(x, y).PercievedIntensity();

			if (grad <= 0.0001)
			{
				// unambigously "left" case
				Put(x, y, source1.Get(x, y));
			}
			else if (grad >= 0.9999)
			{
				// unambigously "right" case
				Put(x, y, source2.Get(x, y));
			}
			else
			{
				// something in between
				LedColor work = LedColor::FadeBetween(
					source1.Get(x, y),
					source2.Get(x, y),
					grad
				);

				if (effect == LedBlendEffect::Sparcle)
				{
					double ints = 1.0 - 2.0 * std::max(0.0, std::min(0.5, fabs(0.5 - grad)));

					if (rand() % 1000 >= 800)
						work = LedColor::FadeBetween(work, LedColors.White, ints);
				}

				Put(x, y, work);
			}
		}

	// OK
	return true;
}

//
//
// ----------------- Diverse --------------------
//
//

// Randomize
LedGradient LedTexture::GetRandomGradient()
{
	int i = rand() % static_cast<int>(LedGradient::__LedGradient_Count);
	return (LedGradient)i;
}

// Randomize
LedBlendEffect LedTexture::GetRandomBlendEffect()
{
	int i = rand() % static_cast<int>(LedBlendEffect::__LedBlendEffect_Count);
	return (LedBlendEffect)i;
}

//
//
// ----------------- Load BMP --------------------
//
//

struct BmpMetaData
{
	unsigned char* Data;			// durch Datei-Lesen gesetzt
	unsigned int FileLen = 0;		// durch Datei-Lesen gesetzt
	unsigned int FileType;			// 2 Bytes an Bytepos = 0, müssen 0x424f sein
	unsigned int PosPixelData;		// 4 Bytes an Bytepos = 10
	unsigned int Width;				// 4 Bytes an Bytepos = 18
	unsigned int Height;			// 4 Bytes an Bytepos = 22
	unsigned int BitCount;			// 2 Bytes an Bytepos = 28
	unsigned int Compression;	    // 4 Bytes an Bytepos = 30
	unsigned int PaletteCount;		// 4 Bytes an Bytepos = 46
};

bool ReadFile(std::string fn, BmpMetaData& img)
{
	// Datei im Stil "C" oeffnen
	FILE* file = fopen(fn.c_str(), "rb");
	if (file == nullptr)
		return false;

	// Dateigroesse erfragen
	fseek(file, 0, SEEK_END);
	img.FileLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Datenpuffer holen
	img.Data = new unsigned char[img.FileLen];

	// Daten lesen und Datei schliessen (!)
	fread(img.Data, img.FileLen, 1, file);
	fclose(file);

	// OK
	return true;
}

unsigned int Decode2Bytes(BmpMetaData& img, int pos)
{
	return img.Data[pos + 1] * 256 + img.Data[pos + 0];
}

unsigned int Decode4Bytes(BmpMetaData& img, int pos)
{
	return
		img.Data[pos + 3] << 24
		| img.Data[pos + 2] << 16
		| img.Data[pos + 1] << 8
		| img.Data[pos + 0];
}

bool DecodeHeader(BmpMetaData& img)
{
	// "Magic number"
	img.FileType = Decode2Bytes(img, 0);
	img.PosPixelData = Decode4Bytes(img, 10);
	img.Width = Decode4Bytes(img, 18);
	img.Height = Decode4Bytes(img, 22);
	img.BitCount = Decode2Bytes(img, 28);
	img.Compression = Decode4Bytes(img, 30);
	img.PaletteCount = Decode4Bytes(img, 46);

	// Ausgabe
	if (false)
	{
		printf("FileType: 0x%04x PosPixelData=%d Width=%d Height=%d BitCount=%d PaletteCount=%d\n",
			img.FileType, img.PosPixelData, img.Width, img.Height, img.BitCount, img.PaletteCount);
	}

	// alles ok?
	if (img.FileType != 0x4d42)
		return false;

	// OK
	return true;
}

bool LedTexture::LoadBmpInternal(std::string fn)
{
	// read
	BmpMetaData header;
	if (!ReadFile(fn, header))
		return false;

	// decode
	if (!DecodeHeader(header))
		return false;

	// what to do with old pixel data?
	int num = mWidth * mHeight;
	if (num > 0)
	{
		mWidth = 0;
		mHeight = 0;
		// assume existing data
		if (mPixels != nullptr)
		{
			delete[] mPixels;
			mPixels = nullptr;
		}
	}

	// allocate enough pixels
	mWidth = header.Width;
	mHeight = header.Height;
	num = mWidth * mHeight;
	if (num > 0)
	{
		mPixels = new LedColor[num];
	}

	// check for different bit formats
	if (header.BitCount == 8)
	{
		// max count for palette
		int maxPI = header.PaletteCount;
		if (maxPI == 0)
			maxPI = 256;

		// the byte size of a row needs to be a multiple of 4
		int rowWidth = (1 + ((header.Width - 1) / 4)) * 4;

		// all pixels
		for (int y = 0; y < mHeight; y++)
			for (int x = 0; x < mWidth; x++)
			{
				// palette index
				uint8_t* src =
					header.Data										// all file data contents 
					+ header.PosPixelData							// pixel data area of file
					+ rowWidth * (header.Height - 1 - y)        	// BMP is Y-flipped!
					+ 1 * x;										// 8 bit palette index

				int pi = *src;

				// allowed index
				if (pi >= maxPI)
					continue;

				// access palette
				unsigned char* rgb = &header.Data[54 + 4 * pi];

				// write pixel
				Put(x, y, LedColor(*(rgb + 2), *(rgb + 1), *(rgb + 0)));
			}
	}
	else
	if (header.BitCount == 24 && header.Compression >= 0)
	{
		// the byte size of a row needs to be a multiple of 4
		int rowWidth = (1 + ((3 * header.Width - 1) / 4)) * 4;

		// all pixels
		for (int y=0; y<mHeight; y++)
			for (int x = 0; x < mWidth; x++)
			{
				// where is the data
				uint8_t* src = 
					header.Data										// all file data contents 
					+ header.PosPixelData							// pixel data area of file
					+ rowWidth * (header.Height - 1 - y)        	// BMP is Y-flipped!
					+ 3 * x ;										// 24 bit is 3 byte
				
				// write pixel
				Put(x, y, LedColor(*(src+2), *(src+1), *(src+0)));
			}
	}

	// no?
	return false;
}
