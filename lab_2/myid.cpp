#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <errno.h>


int main (int argc, char** argv) {

	bool if_name = false;
	if (argc > 1) if_name = true;

	uid_t uid = getuid();
	gid_t gid = getgid();

   	gid_t list[16];
   	int numgr = getgroups(16, list);

	struct passwd *pass;

	if (if_name){
		pass = getpwnam(argv[1]);
           	if (pass == NULL) {
               		perror("no such user name");
               		return EXIT_SUCCESS;
		}
	}
	else{
		pass = getpwuid(uid);
		if (pass == NULL) {
                        perror("fatal");
                        return EXIT_FAILURE;
                }
	}

	struct group *gr = getgrgid(gid);
	gid_t *groups = new gid_t[numgr];
        
	if (groups == NULL) {
        	perror("no groups found");
                return EXIT_FAILURE;
	}
	if (if_name) {
		if (getgrouplist(argv[1], pass->pw_gid, groups, &numgr) == -1) {
               		perror("stderr");
               		return EXIT_FAILURE;
        	}
	}
	else {
                if (getgrouplist(pass->pw_name, pass->pw_gid, groups, &numgr) == -1) {
                        perror("stderr");
                        return EXIT_FAILURE;
                }
        }


	std::cout << "uid=" << uid << "(" << pass->pw_name << ") ";
	std::cout << "gid=" << gid << "(" << gr->gr_name << ") ";
	std::cout << "groups=";

	for (int j = 0; j < numgr; j++) {
		std::cout << groups[j];
		gr = getgrgid(groups[j]);
               	if (gr != NULL)
                	std::cout << "(" << gr->gr_name << ") ";
        }
	std::cout << std::endl;
	delete[] groups;
   	return 0;
}
