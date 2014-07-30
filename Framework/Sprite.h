#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// forward declaration pentru structura SAnimation (folosim pentru a nu include headerul SpriteManager.h in Sprite.h)
struct SAnimation;

// Clasa care va contine tot ce este necesar desenarii unui sprite pe ecran
class CSprite
{
// Variabile logice
	glm::vec3	m_Position;	// pozitia 



// Matricea modelului (contine rotatia, scalarea, translatia) , deocamdata contine doar translatia
	GLuint		m_ModelMatrix_loc; // locatia din shader la care vom trimite matricea de transformare
	GLuint		m_TextureOffset;  // TODO: locatie din shader la care vom trimite offset-ul de texturarea (putem rezolva animatiile si fara sa trimitem tot bufferul de uv inca odata per frame)
								//  folosind uniform de texture offset. (ne bazam ca intr-o animatie nu o sa se modifice dimensiunea sprite-ului)

// Raportul Width/Height 
	float		m_AspectRatio;






//pentru RandomRacket
	float range;
	float pas;


// Identificatori OpengGL pentru resurse grafice
	GLuint		m_ShaderProgram;				// identificatorul shaderului cu care desenam
	GLuint		m_VertexAttributeObjectID;		// identificatorul Vertex attribute objectului
	GLuint		m_PositionVertexBufferID;		// identificatorul bufferului cu pozitii ( x y z)
	GLuint		m_TextureCoordinatesBufferID;	// identificatorul bufferului cu coordonatele de texturare	(u, v)
	GLuint		m_IndexBufferID;				// identificatorul bufferului de indexare

	GLuint		m_TextureID;					// identificatorul texturii cu care se va face desenarea

	// Animation Data
	/* Aceasta structura tine in ea informatiile necesarii afisarii unui sablon de animatie */
	struct CurrentPlayAnimation
	{
		SAnimation * currentAnimationTemplate; // datele incarcate din xml despre animatie -> ne vom referi la ele ca la un sablon/ template 
		float		 currentAnimationPlayTime; // timpul la care s-a ajuns cu afisarea animatiei (in secunde)
		int			 currentFrame;			   // frame-ul la care s-a ajuns cu afisarea animatiei

		// Constructorul care initializeaza structura 
		CurrentPlayAnimation() : currentAnimationTemplate(NULL),currentAnimationPlayTime(0.0f),currentFrame(0)
		{
			// mai sus este o initializare prin lista de initializare
			// ca functionalitate este echivalenta cu 
			// currentAnimationTemplate = NULL ; currentAnimationPlayTime = 0.0f;  currentFrame =0.0f;
		}
	};
	// variabila instatei de sprite care retine informatiile despre animatia care se afiseaza
	CurrentPlayAnimation m_CurrentAnimation;

public:

	//Tipul
	int type;

	//Nr-ul de vieti
	int life;

	//matrice de transformare sprite
	glm::mat4 model_mat;

	//Indicele sprite-ului
	int index;

	float speed;

	CSprite();
	//functie care seteaza tipul sprite-ului
	void SetType(int type);

	// functie care initializeaza sprite-ul -> uploadeaza in memoria placii video bufferele
	void	Init(GLuint shader_programme, float *texture_coordinates, float aspect_ratio);
	
	// functie care se ocupa cu desenarea unui sprite
	void	Draw();
	
	// Update-ul animatiei
	void	UpdateAnimation(float dt);

	// functie care seteaza animatia ce urmeaza sa sa afiseze 
	void	PlayAnimation(const char * name);
	
	// functia care seteaza textura 
	void	SetTexture(int texID) { m_TextureID = texID; }
	
	// functie care seteaza pozitia
	void		SetPosition(glm::vec3 p);

	// functie care intoarce pozitia sprite-ului
	glm::vec3	GetPosition() { return m_Position;}

	~CSprite();
};