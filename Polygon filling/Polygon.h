// Polygon.h


struct Point 
{
	float x;
	float y;
};


class Polygon 
{
private:

	unsigned long int maxVertices;

	unsigned long int numVertices;
	Point* vertices;

public:


	Polygon(int maxVertices) 
	{
		this->initPolygon(maxVertices);
	}

	void initPolygon(int maxVertices); 
	
	void addVertex(float x, float y);
	
	void draw(const bool);

	void drawPoints();

	void drawLabels();

};
