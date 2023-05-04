#include "include/utils.h"

char* get_cwd() {
    char* buf = (char*)malloc(sizeof(char) * 1024);
    add_recycle(buf);

    getcwd(buf, 1024);

    if (NULL == buf) {
        show_error(true, "get current work directory failed");
    }

    return buf;
}

char* get_pwnam(const char* name) {
    struct passwd* pw = getpwnam(name);

    if (NULL == pw) {
        show_error(true, "get user's home directory failed");
    }

    return pw->pw_dir;
}

char* get_kernel_name() {
    struct utsname* ut = (struct utsname*)malloc(sizeof(struct utsname));
    add_recycle(ut);

    if (-1 == uname(ut)) {
        show_error(true, "get kernel name failed");
    }

    return ut->nodename;
}

struct passwd* get_user_record() {
    struct passwd* u = getpwuid(getuid());

    if (NULL == u) {
        show_error(true, "get user's record failed");
    }

    return u;
}

char* get_user_name() {
    return get_user_record()->pw_name;
}

char* get_cur_home_dir() {
    return get_pwnam(get_user_name());
}
