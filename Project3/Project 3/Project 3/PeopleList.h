//
//  PeopleList.hpp
//  Project 3
//
//  Created by Huajun Zhou on 7/19/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#ifndef PeopleList_h
#define PeopleList_h

#include <iostream>

typedef std::string InfoType;


class PeopleList
{
public:
    PeopleList();          // Create an empty In (i.e., one with no InfoType values)
    
    bool empty() const;    // Return true if the list is empty, otherwise false.
    
    int size() const;      // Return the number of elements in the linked list.
    
    bool add(const std::string& firstName, const std::string& lastName, const InfoType& value);
    // If the full name (both the first and last name) is not equal to any full name currently
    // in the list then add it and return true. Elements should be added according to
    // their last name. Elements with the same last name should be added according to
    // their first names. Otherwise, make no change to the list and return false
    // (indicating that the name is already in the list).
    
    bool change(const std::string &firstName, const std::string &lastName, const InfoType& value);
    // If the full name is equal to a full name currently in the list, then make that full
    // name no longer map to the value it currently maps to, but instead map to
    // the value of the third parameter; return true in this case.
    // Otherwise, make no change to the list and return false.
    
    bool addOrChange(std::string firstName, std::string lastName, const InfoType& value);
    // If full name is equal to a name currently in the list, then make that full name no
    // longer map to the value it currently maps to, but instead map to
    // the value of the third parameter; return true in this case.
    // If the full name is not equal to any full name currently in the list then add it
    // and return true. In fact this function always returns true.
    
    bool remove(const std::string& firstName, const std::string& lastName);
    // If the full name is equal to a full name currently in the list, remove the
    // full name and value from the list and return true.  Otherwise, make
    // no change to the list and return false.
    
    bool contains(const std::string firstName, const std::string lastName) const;
    // Return true if the full name is equal to a full name currently in the list, otherwise
    // false.
    
    bool lookup(const std::string& firstName, const std::string& lastName, InfoType& value) const;
    // If the full name is equal to a full name currently in the list, set value to the
    // value in the list that that full name maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    
    bool get(int i, std::string& firstName, std::string& lastName, InfoType& value) const;
    // If 0 <= i < size(), copy into firstName, lastName and value parameters the
    // corresponding information of the element at position i in the list and return
    // true.  Otherwise, leave the parameters unchanged and return false.
    // (See below for details about this function.)
    
    void swap(PeopleList& other);
    // Exchange the contents of this list with the other one.
    ~PeopleList();
    PeopleList (const PeopleList&);
    PeopleList& operator = (const PeopleList&);
private:
    struct Node
    {
        InfoType m_value;
        std::string m_firstName;
        std::string m_lastName;
        Node * next;
        Node * prev;
    };
    Node* head;
    void insert(const std::string& firstName, const std::string& lastName, const InfoType& value, Node * n); // insert a Node after the Node pointed by n.
    };

bool combine(const PeopleList& m1, const PeopleList& m2, PeopleList& result);
void psearch(const std::string& fsearch, const std::string& lsearch, const PeopleList& p1, PeopleList& result);


#endif /* PeopleList_hpp */
