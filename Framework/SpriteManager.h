#ifndef ___H_SPRITE_MANAGER
#define ___H_SPRITE_MANAGER

#include "glm/glm.hpp"

// forward declaration pentru clasa CSprite
class CSprite;

// define-uri  utile pentru SpriteManager
#define MAX_SPRITE_NAME 512
#define MAX_TEXTURE_NAME 512
#define MAX_ANIMATION_NAME 512

#define MAX_LOADED_TEXTURE_COUNT	512
#define MAX_LOADED_SUBTEXTURE_COUNT 1024
#define MAX_ANIMATION_TEMPLATE_COUNT 512
#define MAX_SCENE_SPRITE_COUNT 2048

// In aceasta structura retinem informatiile despre texturile "MARI" care contin "subtexturi"
struct SLoadedTexture
{
	// numele texturii mari ex: "sprites.png"
	char	m_Name[MAX_TEXTURE_NAME];
	
	// identificatorul texturii obtinut de la Opengl
	int		m_Id;
	
	// innaltimeaa texturii
	int		m_Height;
	
	// latimea texturii
	int		m_Width;

	// constructor
	SLoadedTexture() : m_Id	(-1),m_Height(0),m_Width(0)
	{
		m_Name[0]	= 0;
	}
	
};

// In aceasta structura retinem datele pentru sabloanele de animatii incarcate din xml-ul Animations.xml
struct SAnimation
{
	// numele animatiei ex: "Idle" , "Left", "Right"
	char								m_Name[MAX_ANIMATION_NAME];
	
	// offsetul din vectorul de subtexturi de la care incepe animatia
	int									m_BeginFrame;
	
	// offsetul din vectorul de subtexturi la care se termina animatia
	int									m_EndFrame;

	// viteza in FPS (frame-uri pe secunda) la care sa se afiseze animatia
	float								m_Speed;
};


// In aceasta structura se retin informatiile despre o subtextura aflata intr-o textura "MARE"
struct SDictEntry
{
	//	Numele subtexturii
	char				m_Name[MAX_SPRITE_NAME];
	
	// "Parintele" textura (textura mare din care face parte aceasta subtextura)
	SLoadedTexture*		m_RefTexture;
	
	//Retinem pozitia subtexturii in textura intr-un vec4 dupa cum urmeaza:
	//	m_TextureOffsets.x = x   m_TextureOffsets.y= y   m_TextureOffsets.z= width  m_TextureOffsets.w= height 
	glm::vec4			m_TextureOffsets;
	
	//Constructor
	SDictEntry() : m_RefTexture(NULL)
	{
		m_Name[0] = 0;
	}
};

// Clasa care se ocupa cu gestiunea, update-ul si desenarea Sprite-urilor
class CSpriteManager
{
	// vector cu toate animatiile incarcate din Animatii.xml
	SAnimation		m_AllLoadedAnimations	[MAX_ANIMATION_TEMPLATE_COUNT];
	unsigned int	m_NumAllLoadedAnimations;

	// vector cu toate texturile "MARI" in interiorul carora vor fi texturi mici
	SLoadedTexture	m_AllLoadedTextures		[MAX_LOADED_TEXTURE_COUNT];
	unsigned int	m_NumLoadedTextures;

	// vector cu toate texturile mici (subtexturile) incarcate din xml
	SDictEntry		m_AllLoadedSubTextures	[MAX_LOADED_SUBTEXTURE_COUNT];
	unsigned int	m_NumLoadedSubTextures;
	

private:
	// identificatorul shaderului global cu care desenam totul (deocamdata avem un singur shader)
	GLuint			m_GlobalShaderProgram;
	
	// Matricea de proiectie
	glm::mat4		m_ProjectionMatrix;

	// Locatia din shader la care vom uploada matricea de proiectie
	GLuint			m_ProjectionMatrixLoc;
	
	// In aceasta functie se incarca toate sprite-urile 
	void			LoadAllSprites();
	// In aceasta functie se incarca toate animatiile
	void			LoadAllAnimations();

	// Intoarce textura cu numele name
	SLoadedTexture* FindInTextureVector(const char *name);
	// Intoarce o subtextura cu numele name
	SDictEntry*		FindInSubTextureVector( const char * name);

public:
	CSpriteManager();
	~CSpriteManager();


	//vector pentru proiectil
	vector<CSprite*> projectiles;


	//detect collision
	bool check;

	// Initializarea SpriteManagerului
	void		Init();

	// Functie cu care adaugam sprite-uri la desenare ("in scena")
	CSprite *	AddSprite(const char* name, int type);
	// Functie cu care scoatem un sprite din "scena"
	void		RemoveSprite(int id);
	
	// Functie cu care se deseneaza toate sprite-urile "din scena"
	void			Draw();
	// Functie cu care se updateaza toate sprite-urile "din scena"
	void			Update(float dt);

	//Functie care intoarce sablonul de animatie cu numele name
	SAnimation*		GetAnimation(const char *name);

	// Functie care intoarce subtextura de la offsetul id din vectorul de subtexturi 
	SDictEntry *			GetSubTextureAt(int id) {return &m_AllLoadedSubTextures[id];}

	// Getter pentru SpriteManager
	static CSpriteManager * Get() { return &g_me; }

private:

	static CSpriteManager g_me;
};

#endif //___H_SPRITE_MANAGER
