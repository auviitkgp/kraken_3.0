#include <bits/stdc++.h>

using namespace std;
int main(int argc, char const *argv[])
{
	if(argc<3){
		printf("Sorry paths not properly set\n");
		return 0;
	}
	cout<<"source="<<argv[1]<<endl;
	cout<<"destination="<<argv[2]<<endl;


    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);
	std::string input;
	while(cin>>input){
		// cout<<input<<endl;
		if (input=="const")
		{
			cin>>input;
			if(input=="char*")
			{
                char word[250];

				scanf(" %[^;\n]s",word);
				
				printf("%s\n", word);
			}
		}
	}
    
	return 0;
}
