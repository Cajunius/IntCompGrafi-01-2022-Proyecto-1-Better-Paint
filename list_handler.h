#pragma once

#include <colors.h>
#include <status.h>
#include <utils.h>

void incposition(int &pos, list <shared_ptr<CShape>>& l) {
	if (pos < l.size()) {
		list <shared_ptr<CShape>>::iterator it1, it2;
		it1 = l.begin();
		it2 = l.begin();
		advance(it1, pos-1);
		advance(it2, pos);
		
		shared_ptr<CShape> cshape = (*it1);
		(*it1) = (*it2);
		(*it2) = cshape;

		//l.splice(it1, l, it2);

		++pos;
	}
}

void decposition(int& pos, list <shared_ptr<CShape>> &l) {
	if (pos > 1) {
		//pos = pos - 1;
		//incposition(pos, l);
		/*
		list <shared_ptr<CShape>> l2;
		list <shared_ptr<CShape>>::iterator it;
		it = l.begin();
		int i = 1;
		for (; i < pos-1; i++) {
			l2.push_back(*it);
			it++;
		}
		shared_ptr<CShape> cshape = (*it);
		it++;
		l2.push_back(*it);
		l2.push_back(cshape);
		for (; i <= l.size(); i++) {
			l2.push_back(*it);
			it++;
		}
		l = l2;
		*/
		
		list <shared_ptr<CShape>>::iterator it1, it2;
		it1 = l.begin();
		it2 = l.begin();
		advance(it1, pos-1);
		advance(it2, pos-2);
		
		shared_ptr<CShape> cshape = (*it1);
		(*it1) = (*it2);
		(*it2) = cshape;

		//l.splice(it2, l, it1);

		--pos;

		//shared_ptr<CShape> cshape = (*it);
		//it = l.erase(it);
		//--it;
		//l.insert(it, cshape);
	}

}

void toFront(int& pos, list <shared_ptr<CShape>>& l) {
	while (pos < l.size()) {
		incposition(pos, l);
	}
}

void toBack(int& pos, list <shared_ptr<CShape>>& l) {
	while (pos > 1) {
		decposition(pos, l);
	}
}