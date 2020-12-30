/*
 * cfetch: A minimal system information tool for Linux-based operating systems.
 * Copyright (c) 2020 Clint
 * This programme is provided under the GPL-3.0 License. See LICENSE for more details.
 */





#include <stdio.h>


// System information variables
struct sysinfo {
    char username[25], hostname[50], os[50], kernel[50], modelname[50], modelversion[50], cpu[50], shell[8];
    int ramused, ramtotal, apt, flatpak, day, hour, min, sec;
} sysinfo;


// gets the current user
// This will get the current user and the hostname
void getUser() {
    FILE *userName = popen("echo $USER", "r");
    FILE *userHost = popen("cat /proc/sys/kernel/hostname", "r");

    fscanf(userName, "%s", sysinfo.username);
    fscanf(userHost, "%s", sysinfo.hostname);
    fclose(userName);
    fclose(userHost);
}


// gets the distro name
// This will get the name of the distro
void getOS() {
    FILE *distroName = popen("lsb_release -ds 2>/dev/null", "r");

    fscanf(distroName, "%[^\n]s", sysinfo.os);
    fclose(distroName);
}


// gets the kernel
// This will get the kernel-release, kernel-name, and the hardware machin name
void getKernel() {
    FILE *pathKernel = popen("uname -rsm", "r");

    fscanf(pathKernel, "%[^\n]s", sysinfo.kernel);
    fclose(pathKernel);
}


// gets the model information
// This will get the computer's product name and the product version
void getModel() {
    FILE *productName = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    FILE *productVersion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");

    fscanf(productName, "%[^\n]s", sysinfo.modelname);
    fscanf(productVersion, "%s", sysinfo.modelversion);
    fclose(productName);
    fclose(productVersion);
}


// gets the shell information
void getShell() {
    FILE *shellpath = popen("basename $SHELL", "r");

    fscanf(shellpath, "%s", sysinfo.shell);
    fclose(shellpath);
}


// gets the packages
// Supports the distro's package manager, flatpak, and snap
void getPackages() {
    FILE *apt = popen("dpkg-query -f '${binary:Package}\n' -W 2>/dev/null | wc -l", "r");
    FILE *flatpak = popen("flatpak list 2>/dev/null | wc -l", "r");

    fscanf(apt, "%d", &sysinfo.apt);
    fscanf(flatpak, "%d", &sysinfo.flatpak);
    fclose(apt);
    fclose(flatpak);
}


// gets the CPU information
// lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//
// sed -r 's/Model name:\\s{1,}// -> This will remove the 'Model name:', only the CPU will be printed.
void getCPU() {
    FILE *cpuinfo = popen("lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//'", "r");

    fscanf(cpuinfo, "%[^\n]s", sysinfo.cpu);
    fclose(cpuinfo);
}


// gets the RAM information
// This will show the used RAM / total RAM
void getRAM() {
    FILE *used = popen("vmstat -s -S M | grep ' used memory'", "r");
    FILE *total = popen("vmstat -s -S M | grep ' total memory'", "r");

    fscanf(used, "%d", &sysinfo.ramused);
    fscanf(total, "%d", &sysinfo.ramtotal);
    fclose(used);
    fclose(total);
}


// gets the uptime information
// This will show how long the computer is running: DDd, HHh, MMm
void getUptime() {
    FILE *pathUptime = fopen("/proc/uptime", "r");

    fscanf(pathUptime, "%d", &sysinfo.sec);
    fclose(pathUptime);

    sysinfo.day = (sysinfo.sec/60/60/24);
    sysinfo.hour = (sysinfo.sec/60/60%24);
    sysinfo.min = (sysinfo.sec/60%60);
}


// Call all functions
// This will call of the functions that gets the system information
void getsysinfo() {
    getUser();
    getOS();
    getModel();
    getKernel();
    getShell();
    getPackages();
    getCPU();
    getRAM();
    getUptime();
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
    printf("%s\n                   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
    printf("%s                   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
    printf("%s   _____________   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
    printf("%s  |  _________  |  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
    printf("%s  | |         | |  CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
    printf("%s  | |         | |  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
    printf("%s  | |_________| |  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
    printf("%s  |_____________|  PKGS%s   apt: %d, flatpak: %d%s\n", bold, reset, sysinfo.apt, sysinfo.flatpak, reset);
    printf("%s                   UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
    printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);

    return 0;
}
