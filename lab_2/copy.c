#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>

       int main(int argc, char *argv[])
       {
           int ngroups;
           struct passwd *pw;
           struct group *gr;

           if (argc != 3) {
               fprintf(stderr, "Usage: %s <user> <ngroups>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           ngroups = atoi(argv[2]);

           gid_t *groups = (gid_t*) malloc(sizeof(*groups) * ngroups);
           if (groups == NULL) {
               perror("malloc");
               exit(EXIT_FAILURE);
           }

           /* Fetch passwd structure (contains first group ID for user) */

           pw = getpwnam(argv[1]);
           if (pw == NULL) {
               perror("getpwnam");
               exit(EXIT_SUCCESS);
           }

           /* Retrieve group list */

           if (getgrouplist(argv[1], pw->pw_gid, groups, &ngroups) == -1) {
               fprintf(stderr, "getgrouplist() returned -1; ngroups = %d\n",
                       ngroups);
               exit(EXIT_FAILURE);
           }

           /* Display list of retrieved groups, along with group names */

           fprintf(stderr, "ngroups = %d\n", ngroups);
           for (int j = 0; j < ngroups; j++) {
               printf("%d", groups[j]);
               gr = getgrgid(groups[j]);
               if (gr != NULL)
                   printf(" (%s)", gr->gr_name);
               printf("\n");
           }

           exit(EXIT_SUCCESS);
       }
