#ifndef _IMAGE_H
#define _IMAGE_H

#include "Core\Data\Objects\Resources\Resource.h"

#ifndef _COLOR_H
#include "Core/Defines/color.h"
#endif
#ifndef _2DUTILL_H
#include "Core/Defines/d2dutill.h"
#endif

#ifndef _RECT2D_H
	#include "Math/Objects/Rect2D.h"
#endif
#ifndef _MATRIX2D_H
	#include "Math\Objects\Matrix2D.h"
#endif

class Image : public Resource
{
public:
	Image(const std::tstring& resourcePath);
	virtual ~Image();

	virtual bool initialize();
	virtual bool shutdown();

	ID2D1Bitmap* getBitmap() const;
	Rect2D getBounds(const Matrix2D& transform) const;

	int	getWidth() const;
	int	getHeight() const;

	void setOpacity(float opacity);
	float getOpacity() const;
	void setClip(const Rect2D& clip);
	const Rect2D& getClip() const;

	void setTransparencyColor(const Color& transparentColor);

private:
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* renderTargetPtr, IWICImagingFactory* wICFactoryPtr, const std::tstring& uriRef, UINT destinationWidth, UINT destinationHeight, IWICFormatConverter** formatConvertorPtrPtr);

	ID2D1Bitmap*			bitmap;
	IWICFormatConverter*	converter;
	float					opacity;
	Rect2D					clip;
};

#endif