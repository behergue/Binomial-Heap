/*
	binomialHeapFamily.h

	Implementación de una familia de montículos binomiales de mínimos

	Métodos algorítmicos en Resolución de Problemas
	Facultad de Informática
	Universidad Complutense de Madrid

	Beatriz Herguedas Pinedo
*/

#ifndef binomialHeapFamily_h
#define binomialHeapFamily_h

#include <climits>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

//El tipo por defecto de los elementos del montículo es entero
template <class T = int>
class binomialHeapFamily {

protected:

	struct Node;
	using Link = Node * ;

	//Nodo de un montículo
	struct Node {

		//Clave del elemento
		T key;

		//Puntero al padre, al hijo y al hermano de la derecha
		Link parent, child, sibling;

		//Grado del elemento
		size_t degree;

		//Constructor
		Node(T const &k, Link const &p, Link const &c, Link const &s, size_t const &d) :
			key(k), parent(p), child(c), sibling(s), degree(d) {}

		//Constructor vacío
		Node(T const &k) :
			key(k), parent(nullptr), child(nullptr), sibling(nullptr), degree(0) {}

	};

	//Clase montículo interna
	template <class T = int>
	class binomialHeap {

	protected:

		friend class binomialHeapFamily;

		//Puntero al primer elemento del montículo
		Link head;

		//Puntero al elemento mínimo del montículo
		Link min;

		//Número de elementos del montículo
		size_t nelems;

	private:

		/*Constructor*/
		binomialHeap(Link const& h) : head(h) {}

		/*Calcula el mínimo elemento de un montículo*/
		void calculateMin() {

			if (head != nullptr) {
				Link y = nullptr;
				Link x = head;
				min = nullptr;

				if (head != nullptr) {
					min = head;
					y = head;
				}

				while (x != nullptr) {
					if (x->key < min->key) {
						min->key = x->key;
						y = x;
					}
					x = x->sibling;
				}
				min = y;
			}
		}

		/*Fusiona la lista de raíces actual con la de h en una
		sola lista enlazada que se clasifica por grado en orden creciente*/
		void merge(Link const& h) {

			Link x = head;
			Link y = h;
			Link aux1 = nullptr;
			Link aux2 = nullptr;

			if (x->degree < y->degree) {
				aux1 = x;
				x = x->sibling;
			}

			else {
				aux1 = y;
				y = y->sibling;
			}

			aux2 = aux1;

			while (x != nullptr && y != nullptr)
			{
				if (x->degree < y->degree) {
					aux2->sibling = x;
					x = x->sibling;
				}

				else {
					aux2->sibling = y;
					y = y->sibling;
				}

				aux2 = aux2->sibling;
			}

			if (x != nullptr)
				aux2->sibling = x;

			else if (y != nullptr)
				aux2->sibling = y;

			else
				aux2->sibling = nullptr;

			head = aux1;
		}

		/*Convierte al nodo y en el nuevo encabezado de la lista enlazada
		de los hijos de z en tiempo constante*/
		void binomialLink(Link const& y, Link const& z) {
			y->parent = z;
			y->sibling = z->child;
			z->child = y;
			z->degree++;
		}

		/*Une el montículo actual con H*/
		void privateUnion(binomialHeap const& H) {

			if (H.head != nullptr) {

				if (head == nullptr) {
					head = H.head;
					min = H.min;
				}

				else {

					merge(H.head);

					if (head != nullptr) {

						Link prevx = nullptr;
						Link x = head;
						Link nextx = head->sibling;

						while (nextx != nullptr) {

							if (x->degree != nextx->degree ||
								nextx->sibling != nullptr &&
								nextx->sibling->degree == x->degree) {
								prevx = x;
								x = nextx;
							}

							else if (x->key <= nextx->key) {
								x->sibling = nextx->sibling;
								binomialLink(nextx, x);
							}

							else if (prevx == nullptr) {
								head = nextx;
								binomialLink(x, nextx);
								x = nextx;
							}

							else {
								prevx->sibling = nextx;
								binomialLink(x, nextx);
								x = nextx;
							}
							nextx = x->sibling;
						}
					}

					if (H.min->key < min->key)
						min = H.min;
				}
			}
		}

	protected:

		/*Constructor vacío*/
		binomialHeap() : head(nullptr), nelems(0) {}

		/*Constructor*/
		binomialHeap(T const& elem) : nelems(1) {
			head = new Node(elem);
			min = head;
		}

		/*Devuelve el mínimo*/
		T minimum() const {
			return min->key;
		}

		/*Une el montículo actual con H*/
		void unionWith(binomialHeap const& H) {
			privateUnion(H);
			nelems += H.nelems;
		}

		/*Inserta el elemento e en un montículo vacío*/
		Link insert(T const& e) {
			binomialHeap H = binomialHeap(e);
			privateUnion(H);
			nelems++;
			return H.head;
		}

		/*Elimina el elemento mínimo del montículo*/
		T const& extractMin() {

			if (min != nullptr && head != nullptr && min->key == head->key)
				head = min;

			T minim = min->key;
			Link y = min;

			//Elimina el mínimo de la lista de raíces
			Link prevx = nullptr;
			Link x = head;
			while (x != y) {
				prevx = x;
				x = x->sibling;
			}

			if (prevx == nullptr)
				head = x->sibling;

			else
				prevx->sibling = x->sibling;

			//Invierte la lista de hijos del mínimo
			Link z = nullptr;
			Link w = y->child;

			while (w != nullptr) {

				Link next = w->sibling;
				w->sibling = z;
				z = w;
				w = next;
			}

			//Fusiona las dos listas
			binomialHeap H(z);

			H.calculateMin();
			calculateMin();

			privateUnion(H);

			calculateMin();

			nelems--;

			if (nelems == 0) {
				min = nullptr;
				head = nullptr;
			}

			return minim;
		}

		/*Decrece la clave de x a k*/
		void decreaseKey(Link const& x, T const& k) {

			if (k > x->key) {
				throw domain_error("New key is greater than current key");
			}

			x->key = k;
			Link y = x;
			Link z = y->parent;

			while (z != nullptr && y->key < z->key) {
				T aux = y->key;
				y->key = y->parent->key;
				y->parent->key = aux;

				y = z;
				z = y->parent;
			}

			if (min == nullptr || k < min->key)
				min = x;

			if (head == nullptr)
				head = x;
		}

		/*Elimina un elemento*/
		Link deleteKey(Link const& x) {
			T aux = min->key;
			Link minimum = min;
			if (head == nullptr)
				head = min;
			extractMin();
			decreaseKey(x, aux);
			extractMin();
			Link h = insert(aux);
			if (head == minimum)
				head = h;
			return h;
		}

		/*Devuelve el número de elementos del montículo*/
		size_t size() const {
			return nelems;
		}

		/*Comprueba si el montículo está vacío*/
		bool empty() const {
			return nelems == 0;
		}
	};

	using Heap = binomialHeap<T> *;

	//Mapa de cada elemento a un par con su link y su número asociado
	unordered_map<T, pair<Link, int>> elements;

	//Mapa del número asociado a su correspondiente montículo
	unordered_map<int, Heap> heaps;

	//Número de montículos en la familia
	size_t totalElems;

public:

	/*Constructor vacío*/
	binomialHeapFamily() : totalElems(0) {}

	/*Inserta el elemento e en un montículo vacío*/
	void insert(T const& e) {

		auto it = elements.find(e);

		if (it == elements.end()) {
			Heap H = new binomialHeap<T>;
			Link l = H->insert(e);

			heaps.insert({ heaps.size() + 1, H });
			elements.insert({ e,{l, heaps.size()} });

			totalElems++;
		}
	}

	/*Inserta el elemento e en el montículo n*/
	void insert(T const& e, int const& n) {

		if (n <= 0 || n > heaps.size()) {
			throw domain_error("This heap does not exist");
		}

		auto it = elements.find(e);

		if (it == elements.end()) {
			Link l = heaps[n]->insert(e);

			elements.insert({ e, {l, n} });

			totalElems++;
		}
	}

	/*Devuelve el mínimo del montículo n*/
	T const& minimum(int const& n) {

		if (n <= 0 || n > heaps.size()) {
			throw domain_error("This heap does not exist");
		}

		else if (heaps[n]->empty()) {
			throw domain_error("Empty heap");
		}

		return heaps[n]->minimum();
	}

	/*Elimina el mínimo del montículo n*/
	T extractMin(int const& n) {

		if (n <= 0 || n > heaps.size()) {
			throw domain_error("This heap does not exist");
		}

		if (heaps[n]->empty()) {
			throw domain_error("Empty heap");
		}

		totalElems--;
		return heaps[n]->extractMin();
	}

	/*Une los montículos heap1 y heap2 que pertenecen a la misma familia*/
	void unionwith(int const& h1, int const& h2) {

		if (h1 <= 0 || h1 > heaps.size()) {
			throw domain_error("Heap 1 does not exist");
		}
		else if (h1 <= 0 || h1 > heaps.size()) {
			throw domain_error("Heap 2 does not exist");
		}

		else if (h1 == h2) {
			throw domain_error("Heap 1 is equal to heap 2");
		}

		heaps[h1]->unionWith(*heaps[h2]);

		heaps[h2] = new binomialHeap<T>();
	}

	/*Decrece la clave o a n*/
	void decreaseKey(T const& o, T const& n) {

		auto it = elements.find(o);

		if (it == elements.end()) {
			throw domain_error("Key not foud");
		}

		auto it2 = elements.find(n);

		if (it2 != elements.end()) {
			throw domain_error("Key already in the heap family");
		}

		try {
			heaps[elements[o].second]->decreaseKey(elements[o].first, n);

			auto old = elements[o];

			elements.erase(o);

			elements.insert({ n, old });
		}

		catch (domain_error e) {
			throw;
		}
	}

	/*Elimina el elemento e*/
	void deleteKey(T const& e) {

		auto it = elements.find(e);

		if (it == elements.end()) {
			throw domain_error("Unvalid key");
		}

		if (e == heaps[elements[e].second]->min->key)
			heaps[elements[e].second]->extractMin();

		else {
			Link aux = heaps[elements[e].second]->deleteKey(elements[e].first);
			if (elements.size() > 1)
				elements[heaps[elements[e].second]->minimum()] = { aux, elements[e].second };
		}

		elements.erase(e);
		totalElems--;
	}

	/*Devuelve el tamaño del montículo n*/
	size_t size(int const& n) const {

		if (n <= 0 || n > heaps.size()) {
			throw domain_error("This heap does not exist");
		}

		return heaps[n]->size();
	}

	/*Devuelve el tamaño de la familia de montículos*/
	size_t globalSize() const {
		return totalElems;
	}

	/*Comprueba si la familia de montículos está vacía*/
	bool empty() const {
		return totalElems == 0;
	}

	/*Comprueba si el elemento e está en la familia o no*/
	bool isElem(T const& e) {
		return elements.count(e);
	}

	/*Comprueba si el montículo n existe*/
	bool emptyKey(int const& n) {

		if (n <= 0 || n > heaps.size()) {
			throw domain_error("This heap does not exist");
		}

		return heaps[n]->min == nullptr;
	}
};
#endif