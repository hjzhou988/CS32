//
//  main.cpp
//  Project 3
//
//  Created by Huajun Zhou on 7/19/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#include <iostream>
#include "PeopleList.h"
#include <assert.h>
typedef std::string InfoType;

int main()
{
    PeopleList m;
    // For an empty list:
    assert(m.size() == 0);      // test size
    assert(m.empty());          // test empty
    assert(!m.remove("Ricky", "Ricardo")); // nothing to erase
    
    m.add("Sijia","Zheng","34.5"); //Add Nodes with similar last names
    m.add("Siyuan","Zhang", "45");//Add Nodes with similar last names
    m.add("Huajun","Zhou", "33");// Add Nodes with similar last names
    m.add("Yifu","Zheng", "35"); // Add Nodes with similar last names
    
    m.add("Huajun","Zhou","28");// It should not add since it is already in the list.
    m.change("Huajun","Zhou","26");//It should change the value
    assert (!m.add("Yifu","Zheng", "35")); //It should not add since it is already in the list, and it returns false.
    m.addOrChange("Zhiyuan", "Zhang", "29"); //  Test addorchange
    m.addOrChange("Zhiyuan", "Zhang", "35");// It changes the value.
    
    assert(m.size()==5); //size should be 5 now.
    m.remove("Zhiyuan", "Zhang"); //Test remove
    assert(m.size()==4); //Test size again after remove
    assert(m.contains("Yifu","Zheng")); //TEst contains.
    assert(!m.contains("Zhiyuan","Zhang")); //TEst contains.
    
    InfoType v="0";
    assert(!m.lookup("Yifu","Zhang",v)); //Test lookup. Should return false
    assert(m.lookup("Yifu","Zheng",v) && v=="35");//Should return true
    
    assert(!m.lookup("","",v)); //Shouldn't find empty string.
    assert(!m.contains("","")); //Shouldn't find empty string.
    assert(!m.change("","", "45")); //Shouldn't change anything since it does not contain empty string.
    assert(m.addOrChange("","", "45")); //Add an empty string
    assert(m.size()==5); //Size should be 5 now.
    assert(m.addOrChange("","", "440")); //Should change the value
    
    std::string f;
    std::string l;
    
    assert(m.get(0,f,l,v) && f=="" && l=="" && v=="440"); //Test "get".
    assert(!m.get(5,f,l,v)); //Test "get". Returns false since out of range
    assert(v=="440");//v still equals to 440
    
    PeopleList n;
    n.add("Jena","Cleveland", "35");
    m.swap(n);//Test swap
    assert(m.size()==1 && !m.empty()); //Test swapped m size.
    m.swap(n);//Swap back.
    
    PeopleList o(n); //test copy constructor
    PeopleList p=n;//test copy constructor
    o=m;//Test copy assignment. o should change to m.
    o=n; //Test copy assignment. o should change to n.
    o=n=m; //Test copy assignment. o and n should equal to m.
    
    combine (m,n,o); //TEst combine. Since m=n, o should return as m or n.
    combine (m,p,o);//test combine. Now o increases, since m and p are different.
    p.add("Sijia","Zheng", "550"); // Add a node into p with same name but different value;
    assert(!combine(m,p,o));// "combine" should return false since there is a conflict, also "Sijia""Zheng" will be removed.
    
    m.add("Sijia","Zhang","55");
    psearch("Sijia","*", m, o);// should return two people with first name "Sijia".
    psearch("*","Zhang",m,o); // Shoud return two people with last name "zhang"
    psearch ("*","*",m,o); // o should return m
    psearch ("Sijia","Zheng",m,o); //should return "Sijia""Zheng"
    //search ("Siyuan","Zheng",m,o);//Should return nothing
    
    
    for (int n = 0; n < o.size(); n++)
    {
        std::string f;
        std::string l;
        InfoType v;
        o.get(n, f, l, v);
        std::cout << f << " " << l << " " << v << std::endl;
    }
    
    
    return 0;
}
