
struct EnemigoTriangulo{
	int x;
	int y;
	int direccion;
	int tiempo;
	float danno;

	EnemigoTriangulo(){

	}

	EnemigoTriangulo(int _x, int _y, int _direccion, int _tiempo, float _danno){
		x = _x;
		y = _y;
		direccion = _direccion;
		tiempo = _tiempo;
		danno = _danno;
	}
};