#include "main.h"
#include "Restaurant.cpp"
#include <fstream>


void simulate(string filename, imp_res* r)
{
	ifstream ss(filename);
	string str, maxsize, name, energy, num;
	while(ss >> str)
	{ 
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
    	}
        else if(str == "RED") // RED <NAME> <ENERGY>
        {
            ss >> name;
            ss >> energy;
            r->RED(name, stoi(energy));
    	}
    	else if(str == "BLUE") // BLUE <NUM>
    	{
                ss >> num;
    			r->BLUE(stoi(num));
		}
    	else if(str == "PURPLE") // PURPLE
    	{
    			r->PURPLE();
		}
		else if(str == "REVERSAL") // REVERSAL
		{
    			r->REVERSAL();	
		}
    	else if(str == "UNLIMITED_VOID") // UNLIMITED_VOID
     	{   	
    			r->UNLIMITED_VOID();
    	}
    	else if(str == "DOMAIN_EXPANSION") // DOMAIN_EXPANSION
    	{
    			r->DOMAIN_EXPANSION();
    	}
    	else // LIGHT <NUM>
    	{
                ss >> num;
    			r->LIGHT(stoi(num));
    	}
    }
}

int main(int argc, char* argv[]) {
	#ifndef ONLINE_JUDGE;
    freopen("test.txt","r",stdin);
    freopen("outputme.txt","w",stdout);
    #endif
	//if (argc < 2)
    //	return 1;
	//  string fileName = argv[1];
	imp_res* r = new imp_res();
	string fileName = "test.txt";
    simulate(fileName, r);
	//r->InTime();
	//r->InQueue();
  	delete r;
	return 0;
}

/*
TRUE
qd-8
dt-2
rz-8
ri-5
mx--7
ks--4
or--7
pg--8
db--2
kh--4
qg-9
ej-7
ut--4
cj--3
bq--3
pb-2
dg-2
uk-1
if-4
sq-8
qk-1
pz-7
zb-7
yi--3
yf--3
pt-2
yy-4
ft-6
ha--8
di--9
hu--9
io--1
wu--5
tk--5
xy--10





FAIL
dt-2
rz-8
ri-5
mx--7
ks--4
or--7
pg--8
db--2
kh--4
qg-9
ej-7
ut--4
cj--3
bq--3
el-8
pb-2
dg-2
uk-1
if-4
sq-8
qk-1
pz-7
zb-7
yi--3
yf--3
pt-2
yy-4
ft-6
ha--8
di--9
hu--9
io--1
wu--5
tk--5
xy--10
*/