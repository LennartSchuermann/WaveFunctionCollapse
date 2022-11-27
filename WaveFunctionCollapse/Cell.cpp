#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "MathUtility.h"

//Rules: Mountains -> Land -> Coast -> Sea

enum class CellType {
	undefined,		//White
	Mountains,		//Grey
	Land,			//Green
	Coast,			//Yellow
	Sea				//Blue
};

struct Cell {
	sf::RectangleShape shape;
	sf::Vector2f size = { CELLSIZE, CELLSIZE };
	sf::Color color = { 120, 224, 143 };

	CellType type = CellType::undefined;
	Cell* parentCell;

	int entropy = 4;	//everything is possible at 4 -> -1 = collapsed

	std::vector<Cell *> neighbours;

	explicit Cell(sf::Vector2f pos) {
		setColor();

		shape.setPosition(pos);
		shape.setSize(size);
	}

	void collapse(bool isControlled) {
		//define type
		if (isControlled)
			type = static_cast<CellType>(genRandomNumber(1, 4));	//random type for starting cell
		else
			defineType();	//has parent

		//collapse current
		if (type != CellType::undefined) {
			this->entropy = -1;
			this->setColor();

			updateNeighbours();
		}	
	}

	void updateNeighbours() {
		//update neighbours by rules
		for (const auto& neighbourCell : neighbours)
		{
			//TODO more precise updates (use all neighbours to calc entropy)

			neighbourCell->parentCell = this;
			if(neighbourCell->entropy != -1)
				neighbourCell->entropy = neighbourCell->calcEntropy();
		}
	}

	void setColor() {
		switch (type)
		{
		case CellType::Land:
			color = { 29, 209, 161 };
			break;
		case CellType::Sea:
			color = { 10, 189, 227 };
			break;
		case CellType::Coast:
			color = { 254, 202, 87 };
			break;
		case CellType::Mountains:
			color = { 87, 101, 116 };
			break;
		default:
			color = { 200, 214, 229 };
			break;
		}

		shape.setFillColor(color);
	}

	int calcEntropy() const {
		switch (parentCell->type)
		{
		case CellType::Mountains:return 1;
		case CellType::Land:return 2;
		case CellType::Coast:return 2;
		case CellType::Sea:return 1;
		default:return 4;
		}
	}

	void defineType() {
		switch (parentCell->type)
		{
		case CellType::Mountains:
			type = static_cast<CellType>(genRandomNumber(1, 2));	//mountains / land
			break;
		case CellType::Land:
			type = static_cast<CellType>(genRandomNumber(1, 3));	//mountains / land / coast
			break;
		case CellType::Coast:
			type = static_cast<CellType>(genRandomNumber(2, 4));	//land / coast / sea
			break;
		case CellType::Sea:
			type = static_cast<CellType>(genRandomNumber(3, 4));	//coast / sea
			break;
		default:
			type = static_cast<CellType>(genRandomNumber(1, 2));	//mountains / land
			break;
		}
	}
};


struct lessThan
{
	inline bool operator() (Cell const* c1, Cell const* c2) const {
		return (c1->entropy < c2->entropy);
	}
};