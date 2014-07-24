#pragma once

	#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
	#	pragma pack( push, packing )
	#	pragma pack( 1 )
	#	define PACK_STRUCT
	#endif

	struct ChunkHeader
	{
		uint16 id;
		int32 length;
	} PACK_STRUCT;

	// Default alignment
	#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
	#	pragma pack( pop, packing )
	#endif

	#undef PACK_STRUCT

struct ChunkData
{
	ChunkData() : read(0), pos(0) {}

	ChunkHeader header;
	int read;
	int pos;
};

struct SMaterialGroup
{
	SMaterialGroup() : faceCount(0), faces(0) {};

	SMaterialGroup(const SMaterialGroup& o)
	{
		*this = o;
	}

	~SMaterialGroup()
	{
		clear();
	}

	void clear()
	{
		delete [] faces;
		faces = 0;
		faceCount = 0;
	}

	void operator =(const SMaterialGroup& o)
	{
		MaterialName = o.MaterialName;
		faceCount = o.faceCount;
		faces = new uint16[faceCount];
		for (uint32 i=0; i<faceCount; ++i)
			faces[i] = o.faces[i];
	}

	std::string MaterialName;
	uint16 faceCount;
	uint16* faces;
};

struct SCurrentMaterial
{
	SCurrentMaterial()
	{
		clear();
	};

	void clear() {
		Name="";
		Filename[0]="";
		Filename[1]="";
		Filename[2]="";
		Filename[3]="";
		Filename[4]="";
		Strength[0]=0.f;
		Strength[1]=0.f;
		Strength[2]=0.f;
		Strength[3]=0.f;
		Strength[4]=0.f;
		Shininess=0;
		bWireframe=false;
		bBackfaceCulling=false;
		TransparencyMode=TRANSPARENCY_DEFAULT;
		TransparencyValue=1;
		uvScale.x=1;
		uvScale.y=1;
	}

	ColorF AmbientColor;
	ColorF DiffuseColor;
	ColorF SpecularColor;
	float Shininess;
	bool bWireframe;
	bool bBackfaceCulling;
	eTransparencyMode TransparencyMode;
	float TransparencyValue;
	Vec2 uvScale;
	Vec2 uvOffset;


	std::string Name;
	std::string Filename[5];
	f32 Strength[5];
};

class G3DSMeshLoader
{
public:
	G3DSMeshLoader();
	~G3DSMeshLoader(void);

	bool loadMesh(GMesh* pMesh, const char* path, IMaterial* pMaterial = 0);
	bool loadMesh(GMesh* pMesh, FILE *hFile,IMaterial* pMaterial = 0);

	void composeObject(FILE *hFile, const std::string& name);

	void readMeshObject(FILE *hFile, ChunkData* parent);
	void readVertices(FILE *hFile, ChunkData& data);
	void readIndices(FILE *hFile, ChunkData& data);
	void readTexcoords(FILE *hFile, ChunkData& data);
	void readMaterialGroups(FILE *hFile, ChunkData& data);
	bool readMaterialChunk(FILE *hFile, ChunkData* parent);
	void loadMaterials(FILE *hFile);
	bool readColorChunk(FILE *hFile, ChunkData* chunk, ColorF& out);
	bool readPercentageChunk(FILE *hFile, ChunkData* chunk, f32& percentage);

	bool readChunks(FILE *hFile, ChunkData* parent);
	void readString(FILE *hFile, ChunkData& data, std::string& out);
	void readChunkData(FILE *hFile,ChunkData& data);
	void clean();

private:
	GMesh* Mesh;
	IMaterial* pMaterial;

	Matrix44 transformMatr;
	f32* Vertices;
	uint16* Indices;
	f32* TCoords;
	uint16 CountVertices;
	uint16 CountFaces; // = CountIndices/4
	uint16 CountTCoords;

	SCurrentMaterial CurrentMaterial;
	
	std::string loadedFileName;
	std::vector<SMaterialGroup> MaterialGroups;
	std::vector<SCurrentMaterial> Materials;
	
	uint32* SmoothingGroups;
};

