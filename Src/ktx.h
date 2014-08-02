//This file came from the SB6 example code
#ifndef KTX_H
#define KTX_H

#include <QOpenGLFunctions_4_3_Core>

namespace sb6
{

namespace ktx
{

class File : protected QOpenGLFunctions_4_3_Core
{
	unsigned int _load(const char * filename, unsigned int tex = 0);
	bool _save(const char * filename, unsigned int target, unsigned int tex);

public:
	static unsigned int load(const char * filename, unsigned int tex = 0) { return File()._load(filename, tex); }
	static bool save(const char * filename, unsigned int target, unsigned int tex) { return File()._save(filename, target, tex); }


	struct header
	{
		unsigned char       identifier[12];
		unsigned int        endianness;
		unsigned int        gltype;
		unsigned int        gltypesize;
		unsigned int        glformat;
		unsigned int        glinternalformat;
		unsigned int        glbaseinternalformat;
		unsigned int        pixelwidth;
		unsigned int        pixelheight;
		unsigned int        pixeldepth;
		unsigned int        arrayelements;
		unsigned int        faces;
		unsigned int        miplevels;
		unsigned int        keypairbytes;
	};

	union keyvaluepair
	{
		unsigned int        size;
		unsigned char       rawbytes[4];
	};
};

}

}

#endif
