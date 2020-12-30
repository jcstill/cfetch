/*
 * cfetch: A minimal system information tool for Linux-based operating systems.
 * Copyright (c) 2020 Clint
 * This programme is provided under the GPL-3.0 License. See LICENSE for more details.
 */





#include <stdio.h>


// System information variables
struct sysinfo {
    char username[25], hostname[50], os[50], kernel[50], modelname[50], modelversion[50], cpu[50], shell[8];
    int ramused, ramtotal, apt, flatpak, snap, day, hour, min, sec;
} sysinfo;


// gets the current user
// This will get the current user and the hostname
void getuser() {
    FILE *username = popen("echo $USER", "r");
    FILE *hostname = popen("cat /proc/sys/kernel/hostname", "r");

    fscanf(username, "%s", sysinfo.username);
    fscanf(hostname, "%s", sysinfo.hostname);
    fclose(username);
    fclose(hostname);
}


// gets the distro name
// This will get the name of the distro
void getdistro() {
    FILE *os = popen("lsb_release -ds 2>/dev/null", "r");

    fscanf(os, "%[^\n]s", sysinfo.os);
    fclose(os);
}


// gets the kernel
// This will get the kernel-release, kernel-name, and the hardware machin name
void getkernel() {
    FILE *kernel = popen("uname -rsm", "r");

    fscanf(kernel, "%[^\n]s", sysinfo.kernel);
    fclose(kernel);
}


// gets the model information
// This will get the computer's product name and the product version
void getmodel() {
    FILE *modelname = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    FILE *modelversion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");

    fscanf(modelname, "%[^\n]s", sysinfo.modelname);
    fscanf(modelversion, "%s", sysinfo.modelversion);
    fclose(modelname);
    fclose(modelversion);
}


// gets the shell information
void getshell() {
    FILE *shell = popen("basename $SHELL", "r");

    fscanf(shell, "%s", sysinfo.shell);
    fclose(shell);
}


// gets the packages
// Supports the distro's package manager, flatpak, and snap
void getpackages() {
    FILE *apt = popen("dpkg-query -f '${binary:Package}\n' -W 2>/dev/null | wc -l", "r");
    FILE *flatpak = popen("flatpak list 2>/dev/null | wc -l", "r");
    FILE *snap = popen("snap list 2>/dev/null | wc -l", "r");

    fscanf(apt, "%d", &sysinfo.apt);
    fscanf(flatpak, "%d", &sysinfo.flatpak);
    fscanf(snap, "%d", &sysinfo.snap);
    fclose(apt);
    fclose(flatpak);
    fclose(snap);
}


// gets the CPU information
// lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//
// sed -r 's/Model name:\\s{1,}// -> This will remove the 'Model name:', only the CPU will be printed.
void getcpu() {
    FILE *cpu = popen("lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//'", "r");

    fscanf(cpu, "%[^\n]s", sysinfo.cpu);
    fclose(cpu);
}


// gets the RAM information
// This will show the used RAM / total RAM
void getram() {
    FILE *ramused = popen("vmstat -s -S M | grep ' used memory'", "r");
    FILE *ramtotal = popen("vmstat -s -S M | grep ' total memory'", "r");

    fscanf(ramused, "%d", &sysinfo.ramused);
    fscanf(ramtotal, "%d", &sysinfo.ramtotal);
    fclose(ramused);
    fclose(ramtotal);
}


// gets the uptime information
// This will show how long the computer is running: DDd, HHh, MMm
void getuptime() {
    FILE *uptime = fopen("/proc/uptime", "r");

    fscanf(uptime, "%d", &sysinfo.sec);
    fclose(uptime);

    sysinfo.day = (sysinfo.sec/60/60/24);
    sysinfo.hour = (sysinfo.sec/60/60%24);
    sysinfo.min = (sysinfo.sec/60%60);
}


// Call all functions
// This will call of the functions that gets the system information
void getsysinfo() {
    getuser();
    getdistro();
    getmodel();
    getkernel();
    getshell();
    getpackages();
    getcpu();
    getram();
    getuptime();
}


// Colours and text customisation
#define reset "\x1b[0m"
#define bold "\x1b[1m"
#define black "\x1b[30m"
#define red "\x1b[31m"
#define green "\x1b[32m"
#define yellow "\x1b[33m"
#define blue "\x1b[34m"
#define magenta "\x1b[35m"
#define cyan "\x1b[36m"
#define white "\x1b[37m"

int main() {
    getsysinfo();

    // Do not change these
    // user, os, kernel, model, cpu, ram, shell, pkgs, uptime, palette colours
    printf("%s\n                %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
    printf("%s                OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
    printf("%s     ,---._     KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
    printf("%s   /\\`  __ \\\\   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
    printf("%s  |   /    |    CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
    printf("%s  |   \\`.__.\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
    printf("%s   \\            SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
    printf("%s    \\`-,_       PKGS%s   apt: %d, flatpak: %d, snap: %d%s\n", bold, reset, sysinfo.apt, sysinfo.flatpak, sysinfo.snap, reset);
    printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
    printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);

    return 0;
}
