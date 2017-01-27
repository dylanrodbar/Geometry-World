
struct BalaEnemigo{
	int x;
	int y;
	float danno;
	bool activado;

	BalaEnemigo(){

	}

	BalaEnemigo(int _x, int _y, float _danno){
		x = _x;
		y = _y;
		danno = _danno;
		activado = false;
	}
};