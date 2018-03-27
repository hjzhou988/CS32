//
//  PeopleList.cpp
//  Project 3
//
//  Created by Huajun Zhou on 7/19/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#include "PeopleList.h"

PeopleList::PeopleList() //initialize a dummy node pointed by "head".
{
    head= new Node;
    head->next=head;
    head->prev=head;
}

bool PeopleList::empty() const
{
    if (head->next==head)
        return true;
    return false;
}

int PeopleList::size() const
{
    int count=0;
    for (Node* p=head; p->next!=head; p=p->next)
        count++;
    return count;
}

void PeopleList::insert(const std::string& firstName, const std::string& lastName, const InfoType& value, Node* n) // an auxiliary function that inserts a node after the node pointed by n.
{
    Node *p= new Node;
    p->m_value= value;
    p->m_firstName=firstName;
    p->m_lastName=lastName;
    
    p->next=n->next;
    p->prev=n;
    n->next->prev=p;
    n->next=p;
}

bool PeopleList::add(const std::string& firstName, const std::string& lastName, const InfoType& value)
{
    Node* n=head;
    for (; n->next!=head; n=n->next)
    {
        if (lastName <= n->next->m_lastName) break;
    }
    if (n->next==head) // if n goes to the end of the list
    {
        insert(firstName, lastName, value, n);
        return true;
    }
    if (lastName < n->next->m_lastName) // if last name is different.
    {
        insert(firstName, lastName, value, n);
        return true;
    }
    if (lastName == n->next->m_lastName)// if last name is the same
    {
        for (;n->next->m_lastName==lastName; n=n->next) // within the range of same last name
        {
            if (firstName<= n->next->m_firstName) break;
        }
        if(n->next->m_lastName!=lastName) // if n goes to the end of the same last name category, insert
        {
            insert(firstName, lastName, value, n);
            return true;
        }
        
        if (firstName< n->next->m_firstName) //if n finds the position less than a first name.
        {
            insert(firstName, lastName, value, n);
            return true;
        }
        return false; // when same first and last name was found in the list
    }
    return false;
}
bool PeopleList::change(const std::string &firstName, const std::string &lastName, const InfoType& value)
{
    Node* n = head;
    for (; n->next!=head; n=n->next)
    {
        if (lastName == n->next->m_lastName && firstName==n->next->m_firstName && value!=n->next->m_value) break;
    }
    
    if (n->next==head) // didn't find a same first and last name with a different value
        return false;
        
    n->next->m_value=value; // else changed the value.
    return true;
}
bool PeopleList::addOrChange(std::string firstName, std::string lastName, const InfoType& value)
{
    Node* n=head;
    for (; n->next!=head; n=n->next)
    {
        if (lastName <= n->next->m_lastName) break;
    }
    if (n->next==head)// when n goes to the end of the list,  which means this lastName will be put at the end of the people list
    {
        insert(firstName, lastName, value, n);
        return true;
    }
    if (lastName < n->next->m_lastName)// insert a Node after n and before n->next
    {
        insert(firstName, lastName, value, n);
        return true;
    }
    
    // Below is the case when there is a same lastname in the list already
    for (;n->next->m_lastName==lastName; n=n->next)
    {
        if (firstName<= n->next->m_firstName) break;
    }
    
    if(n->next->m_lastName!=lastName) // when firstname needs to be inserted into the end of that last name category
    {
        insert(firstName, lastName, value, n);
        return true;
    }
    
    if (firstName< n->next->m_firstName)// when firstname needs to be inserted before a larger first name.
    {
        insert(firstName, lastName, value, n);
        return true;
    }
    //below is the case when there is a Node in the list having the same last and first name with the given argument.
    n->next->m_value=value;
    return true;
}

bool PeopleList::remove(const std::string& firstName, const std::string& lastName)
{
    Node* n = head;
    for (; n->next!=head; n=n->next)
    {
        if (lastName == n->next->m_lastName && firstName==n->next->m_firstName) break;
    }
    if (n->next==head) // didn't find the node to remove
        return false;
    
    Node *p=n->next; // creat a pointer that points to the node that will be removed
    n->next=p->next;
    n->next->prev=n;
    delete p;
    return true;
}
bool PeopleList::contains(const std::string firstName, const std::string lastName) const
{
    Node* n = head;
    for (; n->next!=head; n=n->next)
    {
        if (lastName == n->next->m_lastName && firstName==n->next->m_firstName) break;
    }
    if (n->next==head) return false;
    return true;
}

bool PeopleList::lookup(const std::string& firstName, const std::string& lastName, InfoType& value) const
{
    Node* n = head;
    for (; n->next!=head; n=n->next)
    {
        if (lastName == n->next->m_lastName && firstName==n->next->m_firstName) break;
    }
    if (n->next==head)
        return false;

    value=n->next->m_value;
    return true;
}
bool PeopleList::get(int i, std::string& firstName, std::string& lastName, InfoType& value) const
{
    if (i>=0 && i<size())
    {
        Node * n=head;
        int count=0;
        for (;n->next!=head;n=n->next)
        {
            count++;
            if (i==count-1) // because count++ first, the index need to be subtracted by 1 to be compared to i.
            {
                firstName=n->next->m_firstName;
                lastName=n->next->m_lastName;
                value=n->next->m_value;
                return true;
            }
        }
    }
    return false;
}
void PeopleList::swap(PeopleList& other)
{
    Node *temp=head;
    head=other.head;
    other.head=temp;
}

PeopleList::~PeopleList()
{
    while (head->next!=head) // destruct nodes one by one
    {
        Node *p=head;
        head=head->next;
        head->prev=p->prev;
        p->prev->next=head;
        delete p;
    }
    delete head; // finally destruct the dummy node pointed by head
}

PeopleList::PeopleList (const PeopleList& other)
{
    head= new Node;
    head->next=head;
    head->prev=head;
    Node *p=head;
    
    for (Node* n=other.head; n->next!=other.head; n=n->next)
    {
        insert(n->next->m_firstName, n->next->m_lastName, n->next->m_value, p);
        p=p->next;
    }
}

PeopleList& PeopleList::operator = (const PeopleList&other)
{
    PeopleList copy(other);
    swap(copy);
    return *this;
}

bool combine(const PeopleList& m1, const PeopleList& m2, PeopleList& result)
{
    PeopleList newresult=m1; //Initialize a new result that's the copy of m1
    int event=0; //Initialize an integer to record the number of Nodes with same first and last names but different values in m1 and m2.
    for (int i=0; i<m2.size();i++) //check every node in m2
    {
        std::string f;
        std::string l;
        InfoType v;
        m2.get(i,f,l,v);
        InfoType tempv=v; // we need to see whether v changes in m1, so assign v to a temporary v
        if(m1.lookup(f,l,v)) //if m1 contains the first and last name from m2
        {
           
            if (tempv==v)//if v are the same in both m1 and m2
            {
                continue;
            }
            else // if v are different, increase event and continue
            {
                newresult.remove(f,l);
                event++;continue;
            }
        }
        else newresult.add(f,l,v);
    }
    result= newresult;
    if (event>0) return false;
    else return true;
}
void psearch(const std::string& fsearch, const std::string& lsearch, const PeopleList& p1, PeopleList& result)
{
    PeopleList newresult;
    InfoType v;
    
    if (fsearch=="*" && lsearch!="*")
    {
        std::string f;
        std::string l;
        
        for (int i=0; i<p1.size();i++)
        {
            p1.get(i,f,l,v);
            if (l==lsearch)
                newresult.add(f,l,v);
        }
    }
    
     if (fsearch!="*" && lsearch=="*")
     {
         std::string f;
         std::string l;
         for (int i=0; i<p1.size();i++)
         {
             p1.get(i,f,l,v);
             if (f==fsearch)
                 newresult.add(f,l,v);
         }
     }
    if (fsearch=="*" && lsearch=="*")
        newresult=p1;
    
    if (p1.lookup(fsearch,lsearch,v))// if full name matches,
    {
        newresult.add(fsearch,lsearch,v);
    }
    result=newresult;// replace result with new result. The case when no matches was also included, and it will return empty list, which is a default constructor for PeopleList
}













