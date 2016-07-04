#include <string>
#include <fstream>
#include "d3dx12.h"

struct FontChar
{
	// the unicode id
	int id;

	// these need to be converted to texture coordinates 
	// (where 0.0 is 0 and 1.0 is textureWidth of the font)
	float u; // u texture coordinate
	float v; // v texture coordinate
	float twidth; // width of character on texture
	float theight; // height of character on texture

	float width; // width of character in screen coords
	float height; // height of character in screen coords

				  // these need to be normalized based on size of font
	float xoffset; // offset from current cursor pos to left side of character
	float yoffset; // offset from top of line to top of character
	float xadvance; // how far to move to right for next character
};

struct FontKerning
{
	int firstid; // the first character
	int secondid; // the second character
	float amount; // the amount to add/subtract to second characters x
};

struct Font
{
	std::wstring name; // name of the font
	std::wstring fontImage;
	int size; // size of font, lineheight and baseheight will be based on this as if this is a single unit (1.0)
	float lineHeight; // how far to move down to next line, will be normalized
	float baseHeight; // height of all characters, will be normalized
	int textureWidth; // width of the font texture
	int textureHeight; // height of the font texture
	int numCharacters; // number of characters in the font
	FontChar* CharList; // list of characters
	int numKernings; // the number of kernings
	FontKerning* KerningsList; // list to hold kerning values
	ID3D12Resource* textureBuffer; // the font texture resource
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandle; // the font srv

										   // these are how much the character is padded in the texture. We
										   // add padding to give sampling a little space so it does not accidentally
										   // padd the surrounding characters. We will need to subtract these paddings
										   // from the actual spacing between characters to remove the gaps you would otherwise see
	float leftpadding;
	float toppadding;
	float rightpadding;
	float bottompadding;

	// this will return the amount of kerning we need to use for two characters
	float GetKerning(wchar_t first, wchar_t second)
	{
		for (int i = 0; i < numKernings; ++i)
		{
			if ((wchar_t)KerningsList[i].firstid == first && (wchar_t)KerningsList[i].secondid == second)
				return KerningsList[i].amount;
		}
		return 0.0f;
	}

	// this will return a FontChar given a wide character
	FontChar* GetChar(wchar_t c)
	{
		for (int i = 0; i < numCharacters; ++i)
		{
			if (c == (wchar_t)CharList[i].id)
				return &CharList[i];
		}
		return nullptr;
	}
};


class text {
	
public:

	Font LoadFont(LPCWSTR filename, int windowWidth, int windowHeight)
	{
		std::wifstream fs;
		fs.open(filename);

		Font font;
		std::wstring tmp;
		int startpos;

		// extract font name
		fs >> tmp >> tmp; // info face="Arial"
		startpos = tmp.find(L"\"") + 1;
		font.name = tmp.substr(startpos, tmp.size() - startpos - 1);

		// get font size
		fs >> tmp; // size=73
		startpos = tmp.find(L"=") + 1;
		font.size = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

		// bold, italic, charset, unicode, stretchH, smooth, aa, padding, spacing
		fs >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp; // bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 

															 // get padding
		fs >> tmp; // padding=5,5,5,5 
		startpos = tmp.find(L"=") + 1;
		tmp = tmp.substr(startpos, tmp.size() - startpos); // 5,5,5,5

														   // get up padding
		startpos = tmp.find(L",") + 1;
		font.toppadding = std::stoi(tmp.substr(0, startpos)) / (float)windowWidth;

		// get right padding
		tmp = tmp.substr(startpos, tmp.size() - startpos);
		startpos = tmp.find(L",") + 1;
		font.rightpadding = std::stoi(tmp.substr(0, startpos)) / (float)windowWidth;

		// get down padding
		tmp = tmp.substr(startpos, tmp.size() - startpos);
		startpos = tmp.find(L",") + 1;
		font.bottompadding = std::stoi(tmp.substr(0, startpos)) / (float)windowWidth;

		// get left padding
		tmp = tmp.substr(startpos, tmp.size() - startpos);
		font.leftpadding = std::stoi(tmp) / (float)windowWidth;

		fs >> tmp; // spacing=0,0

				   // get lineheight (how much to move down for each line), and normalize (between 0.0 and 1.0 based on size of font)
		fs >> tmp >> tmp; // common lineHeight=95
		startpos = tmp.find(L"=") + 1;
		font.lineHeight = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowHeight;

		// get base height (height of all characters), and normalize (between 0.0 and 1.0 based on size of font)
		fs >> tmp; // base=68
		startpos = tmp.find(L"=") + 1;
		font.baseHeight = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowHeight;

		// get texture width
		fs >> tmp; // scaleW=512
		startpos = tmp.find(L"=") + 1;
		font.textureWidth = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

		// get texture height
		fs >> tmp; // scaleH=512
		startpos = tmp.find(L"=") + 1;
		font.textureHeight = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

		// get pages, packed, page id
		fs >> tmp >> tmp; // pages=1 packed=0
		fs >> tmp >> tmp; // page id=0

						  // get texture filename
		std::wstring wtmp;
		fs >> wtmp; // file="Arial.png"
		startpos = wtmp.find(L"\"") + 1;
		font.fontImage = wtmp.substr(startpos, wtmp.size() - startpos - 1);

		// get number of characters
		fs >> tmp >> tmp; // chars count=97
		startpos = tmp.find(L"=") + 1;
		font.numCharacters = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

		// initialize the character list
		font.CharList = new FontChar[font.numCharacters];

		for (int c = 0; c < font.numCharacters; ++c)
		{
			// get unicode id
			fs >> tmp >> tmp; // char id=0
			startpos = tmp.find(L"=") + 1;
			font.CharList[c].id = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

			// get x
			fs >> tmp; // x=392
			startpos = tmp.find(L"=") + 1;
			font.CharList[c].u = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)font.textureWidth;

			// get y
			fs >> tmp; // y=340
			startpos = tmp.find(L"=") + 1;
			font.CharList[c].v = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)font.textureHeight;

			// get width
			fs >> tmp; // width=47
			startpos = tmp.find(L"=") + 1;
			tmp = tmp.substr(startpos, tmp.size() - startpos);
			font.CharList[c].width = (float)std::stoi(tmp) / (float)windowWidth;
			font.CharList[c].twidth = (float)std::stoi(tmp) / (float)font.textureWidth;

			// get height
			fs >> tmp; // height=57
			startpos = tmp.find(L"=") + 1;
			tmp = tmp.substr(startpos, tmp.size() - startpos);
			font.CharList[c].height = (float)std::stoi(tmp) / (float)windowHeight;
			font.CharList[c].theight = (float)std::stoi(tmp) / (float)font.textureHeight;

			// get xoffset
			fs >> tmp; // xoffset=-6
			startpos = tmp.find(L"=") + 1;
			font.CharList[c].xoffset = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowWidth;

			// get yoffset
			fs >> tmp; // yoffset=16
			startpos = tmp.find(L"=") + 1;
			font.CharList[c].yoffset = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowHeight;

			// get xadvance
			fs >> tmp; // xadvance=65
			startpos = tmp.find(L"=") + 1;
			font.CharList[c].xadvance = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowWidth;

			// get page
			// get channel
			fs >> tmp >> tmp; // page=0    chnl=0
		}

		// get number of kernings
		fs >> tmp >> tmp; // kernings count=96
		startpos = tmp.find(L"=") + 1;
		font.numKernings = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

		// initialize the kernings list
		font.KerningsList = new FontKerning[font.numKernings];

		for (int k = 0; k < font.numKernings; ++k)
		{
			// get first character
			fs >> tmp >> tmp; // kerning first=87
			startpos = tmp.find(L"=") + 1;
			font.KerningsList[k].firstid = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

			// get second character
			fs >> tmp; // second=45
			startpos = tmp.find(L"=") + 1;
			font.KerningsList[k].secondid = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

			// get amount
			fs >> tmp; // amount=-1
			startpos = tmp.find(L"=") + 1;
			int t = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos));
			font.KerningsList[k].amount = (float)t / (float)windowWidth;
		}

		return font;
	}
};

