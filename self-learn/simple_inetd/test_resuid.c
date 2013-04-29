#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


typedef void (*TEST_RESUID_FUNC)(gid_t, gid_t, gid_t);


struct FuncMap {
  char *name;
  TEST_RESUID_FUNC func;
};


void print_help() {
  char *form1 = "./a.out setuid 'effective username'\n";
  char *form2 = "./a.out seteuid 'effective username'\n";
  char *form3 = "./a.out setreuid 'real username' 'effective username'\n";
  char *form4 = "./a.out setresuid 'real username' 'effective username' 'saved username'\n";
  printf("%s%s%s%s", form1, form2, form3, form4);
  exit(EXIT_SUCCESS);
}


void call_setuid(gid_t ruid, gid_t euid, gid_t suid) {
  if (setuid(euid) == -1) {
    printf("setuid(%d):%s\n", euid, strerror(errno));
  }
}


void call_seteuid (gid_t ruid, gid_t euid, gid_t suid) {
  if (seteuid(euid) == -1) {
    printf("seteuid(%d):%s\n", euid, strerror(errno));
  }
}


void call_setreuid (gid_t ruid, gid_t euid, gid_t suid) {
  setreuid(ruid, euid);
}


void call_setresuid (gid_t ruid, gid_t euid, gid_t suid) {
  setresuid(ruid, euid, suid);

}


void print_resuid() {
  gid_t ruid, euid, suid;
  if (getresuid(&ruid, &euid, &suid) == 0)
    printf("ruid=%d, euid=%d, suid=%d\n", ruid, euid, suid);
}


int main(int argc, char *argv[]) {
  gid_t ruid, euid, suid;
  struct FuncMap map[4];
  struct passwd *r_pwd, *e_pwd, *s_pwd;

  map[0].name = "setuid";
  map[0].func = &call_setuid;

  map[1].name = "seteuid";
  map[1].func = &call_seteuid;

  map[2].name = "setreuid";
  map[2].func = &call_setreuid;

  map[3].name = "setresuid";
  map[3].func = &call_setresuid;

  if (argc < 2) print_help();

  char *name = argv[1];

  if (strcmp(name, "setuid") == 0) {
    if (argc < 3) print_help();

    // getpwnam is not reentrant.
    e_pwd = getpwnam(argv[2]);
    if (e_pwd == NULL) {
      printf("%s is not a valid user!\n", argv[2]);
      exit(EXIT_FAILURE);
    }
    euid = e_pwd->pw_uid;

    printf("Before call %s(%d)\n", argv[1], euid);
    print_resuid();
    map[0].func(ruid, e_pwd->pw_uid, suid);
    printf("After call %s(%d)\n", argv[1], euid);
    print_resuid();

  } else if (strcmp(name, "seteuid") == 0) {
    if (argc < 3) print_help();

    e_pwd = getpwnam(argv[2]);
    if (e_pwd == NULL) {
      printf("%s is not a valid user!\n", argv[2]);
      exit(EXIT_FAILURE);
    }
    euid = e_pwd->pw_uid;

    printf("Before call %s(%d)\n", argv[1], euid);
    print_resuid();
    map[1].func(ruid, euid, suid);
    printf("After call %s(%d)\n", argv[1], euid);
    print_resuid();

  } else if (strcmp(name, "setreuid") == 0) {
    if (argc < 4) print_help();

    r_pwd = getpwnam(argv[2]);
    if (r_pwd == NULL) {
      printf("%s is not a valid user!, set ruid to -1\n", argv[2]);
      ruid = -1;
    } else {
      ruid = r_pwd->pw_uid;
    }

    e_pwd = getpwnam(argv[3]);
    if (e_pwd == NULL) {
      printf("%s is not a valid user! set euid to -1\n", argv[3]);
      euid = -1;
    } else {
      euid = e_pwd->pw_uid;
    }


    printf("Before call %s(%d, %d)\n", argv[1], ruid, euid);
    print_resuid();
    map[2].func(ruid, euid, suid);
    printf("After call %s(%d, %d)\n", argv[1], ruid, euid);
    print_resuid();

  } else if (strcmp(name, "setresuid") == 0) {
    if (argc < 5) print_help();

    r_pwd = getpwnam(argv[2]);
    if (r_pwd == NULL) {
      printf("%s is not a valid user! set ruid to -1\n", argv[2]);
      ruid = -1;
    } else {
      ruid = r_pwd->pw_uid;
    }

    e_pwd = getpwnam(argv[3]);
    if (e_pwd == NULL) {
      printf("%s is not a valid user!\n", argv[3]);
      exit(EXIT_FAILURE);
    }
    euid = e_pwd->pw_uid;

    s_pwd = getpwnam(argv[4]);
    if (s_pwd == NULL) {
      printf("%s is not a valid user!\n", argv[4]);
      exit(EXIT_FAILURE);
    }
    suid = s_pwd->pw_uid;

    printf("Before call %s(%d, %d, %d)\n", argv[1], ruid, euid, suid);
    print_resuid();
    map[3].func(ruid, euid, suid);
    printf("After call %s(%d, %d, %d)\n", argv[1], ruid, euid, suid);
    print_resuid();

  } else {
    print_help();
  }

  exit(EXIT_SUCCESS);

  if (getresuid(&ruid, &euid, &suid) == 0)
    // If run as a normal user, and there is no setuid bit set for other user.
    // Output looks like:
    // ruid=52033, euid=52033, suid=52033
    // If run as a normal user, and there is setuid bit set for other user.
    // Output looks like:
    // ruid=52033, euid=119, suid=119 (setuid to sshd)
    // ruid=52033, euid=0, suid=0 (setuid to root)
    printf("ruid=%d, euid=%d, suid=%d\n", ruid, euid, suid);


}
