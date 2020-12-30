/*
 * cfetch: A minimal system information tool for Linux-based operating systems.
 * Copyright (c) 2020 Clint
 * This programme is provided under the GPL-3.0 License. See LICENSE for more details.
 */

#include <stdio.h>
#include <string.h>

// Colours and text customization
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

// System information variables
struct systemInfo {
    char username[25], hostname[50], os[50], distro[50], kernel[50], modelname[50], modelversion[50], cpu[50], shell[8];
    int day, hour, min, sec, ramused, ramtotal;
    int dpkg, flatpak, snap, pacman, kiss, dnf, portage;
} sysInfo;

// Detects the current user
// This will detect the current user and the hostname
void detectUser() {
    FILE *userName = popen("echo $USER", "r");
    FILE *userHost = popen("cat /proc/sys/kernel/hostname", "r");

    fscanf(userName, "%s", sysInfo.username);
    fscanf(userHost, "%s", sysInfo.hostname);
    fclose(userName);
    fclose(userHost);
}

// Detects the distro name
// This will detect the name of the distro
void detectDistro() {
    FILE *osName = popen("if [ $(command -v lsb_release) ];then lsb_release -ds;else cat /etc/*-release | grep 'PRETTY_NAME=' | cut -d '\"' -f2;fi 2>/dev/null", "r");
    fscanf(osName, "%[^\n]s", sysInfo.os);
    fclose(osName);

    FILE *distroName = popen("if [ $(command -v lsb_release) ];then lsb_release -is;else cat /etc/*-release | grep 'DISTRIB_ID=' |cut -d '=' -f2;fi 2>/dev/null", "r");
    fscanf(distroName, "%[^\n]s", sysInfo.distro);
    fclose(distroName);
}

// Detects the kernel
// This will detect the kernel-release, kernel-name, and the hardware machin name
void detectKernel() {
    FILE *pathKernel = popen("uname -rsm", "r");

    fscanf(pathKernel, "%[^\n]s", sysInfo.kernel);
    fclose(pathKernel);
}

// Detects the model information
// This will detect the computer's product name and the product version
void detectModel() {
    FILE *productName = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    fscanf(productName, "%[^\n]s", sysInfo.modelname);
    fclose(productName);
    FILE *productVersion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
    fscanf(productVersion, "%[^\n]s", sysInfo.modelversion);
    fclose(productVersion);
}

// Detects the shell information
void detectShell() {
    FILE *shellpath = popen("basename $SHELL", "r");

    fscanf(shellpath, "%s", sysInfo.shell);
    fclose(shellpath);
}

// Detects the packages
// Currently this works with dpkg, flatpak, snap, pacman, and kiss;
void detectPackages() {
    FILE *dpkgs = popen("if [ $(command -v dpkg-query) ];then dpkg-query -l | wc -l;fi 2>/dev/null", "r");
    fscanf(dpkgs, "%d", &sysInfo.dpkg);
    fclose(dpkgs);

    FILE *flatpaks = popen("if [ $(command -v flatpak) ];then flatpak list | wc -l;fi 2>/dev/null", "r");
    fscanf(flatpaks, "%d", &sysInfo.flatpak);
    fclose(flatpaks);

    FILE *snaps = popen("if [ $(command -v snap) ];then snap list | wc -l;fi 2>/dev/null", "r");
    fscanf(snaps, "%d", &sysInfo.snap);
    fclose(snaps);

    FILE *pacmans = popen("if [ $(command -v pacman) ];then pacman -Q | wc -l;fi 2>/dev/null", "r");
    fscanf(pacmans, "%d", &sysInfo.pacman);
    fclose(pacmans);

    FILE *kisses = popen("if [ $(command -v kiss) ];then kiss list | wc -l;fi 2>/dev/null", "r");
    fscanf(kisses, "%d", &sysInfo.kiss);
    fclose(kisses);

    FILE *dnf = popen("if [ $(command -v dnf) ];then dnf list installed | wc -l;fi 2>/dev/null", "r");
    fscanf(dnf, "%d", &sysInfo.dnf);
    fclose(dnf);
}

// Detects the CPU information
// lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//
// sed -r 's/Model name:\\s{1,}// -> This will remove the 'Model name:', only the CPU will be printed.
void detectCPU() {
    FILE *cpuinfo = popen("lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//'", "r");

    fscanf(cpuinfo, "%[^\n]s", sysInfo.cpu);
    fclose(cpuinfo);
}

// Detects the RAM information
// This will show the used RAM / total RAM
void detectRAM() {
    FILE *used = popen("vmstat -s -S M | grep ' used memory'", "r");
    FILE *total = popen("vmstat -s -S M | grep ' total memory'", "r");

    fscanf(used, "%d", &sysInfo.ramused);
    fscanf(total, "%d", &sysInfo.ramtotal);
    fclose(used);
    fclose(total);
}

// Detects the uptime information
// This will show how long the computer is running: DDd, HHh, MMm
void detectUptime() {
    FILE *pathUptime = fopen("/proc/uptime", "r");

    fscanf(pathUptime, "%d", &sysInfo.sec);
    fclose(pathUptime);

    sysInfo.day = (sysInfo.sec/60/60/24);
    sysInfo.hour = (sysInfo.sec/60/60%24);
    sysInfo.min = (sysInfo.sec/60%60);
}

// Print the correct packages and names
void printpkgs(){
    int comma=0;
    if(sysInfo.dpkg != 0){
        if(comma == 1)printf(", ");
        printf("%d(dpkg)", sysInfo.dpkg);
        comma=1;
    }
    if(sysInfo.flatpak != 0){
        if(comma == 1)printf(", ");
        printf("%d(flatpak)", sysInfo.flatpak);
        comma=1;
    }
    if(sysInfo.snap != 0){
        if(comma == 1)printf(", ");
        printf("%d(snap)", sysInfo.snap);
        comma=1;
    }
    if(sysInfo.pacman != 0){
        if(comma == 1)printf(", ");
        printf("%d(pacman)", sysInfo.pacman);
        comma=1;
    }
    if(sysInfo.kiss != 0){
        if(comma == 1)printf(", ");
        printf("%d(kiss)", sysInfo.kiss);
        comma=1;
    }
    printf("%s\n", reset);
}

// Call all functions
// This will call of the functions that detects the system information
void getSysinfo() {
    detectUser();
    detectDistro();
    detectModel();
    detectKernel();
    detectShell();
    detectPackages();
    detectCPU();
    detectRAM();
    detectUptime();
}


int main() {
    getSysinfo();

    // Do not change these
    // user, os, kernel, model, cpu, ram, shell, pkgs, uptime, palette colours
    if(strcmp(sysInfo.distro, "Arch") == 0){
        printf("%s\n                  %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s        /\\        OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s       /  \\       KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s      /\\   \\      MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s     /  __  \\     CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s    /  (  )  \\    RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s   / __|  |__ \\   SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s  /.\\`      \\`.\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "Debian") == 0){
        printf("%s\n                %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s                OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s     ,---._     KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s   /\\`  __ \\\\   MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s  |   /    |    CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s  |   \\`.__.\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s   \\            SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s    \\`-,_       PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "Elementary") == 0){
        printf("%s\n               %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s               OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s    _______    KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s   /  ___  \\   MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s  /  |  /  /\\  CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s  |  \\ /  / |  RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s  \\   /__/  /  SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s   \\_______/   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s               UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s               %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "Fedora") == 0){
        printf("%s\n                  %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s        _____     OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s       /   __)\\   KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s       |  /  \\ \\  MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s    ___|  |__/ /  CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s   / (_    _)_/   RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s  / /  |  |       SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s  \\ \\__/  |       PKGS%s   dnf: %d, flatpak: %d, snap: %d%s\n", bold, reset, sysInfo.dnf, sysInfo.flatpak, sysInfo.snap, reset);
        printf("%s   \\(_____/       UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "Gentoo") == 0){
        printf("%s\n                  %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s    .-----.       OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s  .\\`    _  \\`.   KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s  \\`.   ()   \\`.  MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s    \\`.        /  CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s   .\\`       .\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s  /       .\\`     SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s  \\____.-\\`       PKGS%s   %d(portage)%s\n", bold, reset, sysInfo.portage, reset);
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "Lite") == 0){
        printf("%s\n          %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s     /\\   OS%s     %s%s\n", bold, reset,sysInfo. os, reset);
        printf("%s    /  \\  KERNEL%s %s%s\n", bold, reset,sysInfo. kernel, reset);
        printf("%s   / / /  MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s  > / /   CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s  \\ \\ \\   RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s   \\_\\_\\  SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s      \\   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s          UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s               %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "Mint") == 0){
        printf("%s\n                   %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s   _____________   OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s  |_            \\  KERNEL%s %s%s\n", bold, reset,sysInfo. kernel, reset);
        printf("%s    |  |  ___   |  MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s    |  | | | |  |  CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s    |  | | | |  |  RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s    |   \\____/  |  SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s    \\__________/   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                   UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "PopOS") == 0){
        printf("%s\n                     %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s  ______             OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s  \\   _ \\        __  KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s   \\ \\ \\ \\      / /  MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s    \\ \\_\\ \\    / /   CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s     \\  ___\\  /_/    RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s      \\ \\    _       SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s     __\\_\\__(_)_     PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s    (___________)    UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s                     %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "Raspbian") == 0){
        printf("%s\n              %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s              OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s    __  __    KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s   (_\\)(/_)   MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s   (_(__)_)   CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s  (_(_)(_)_)  RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s   (_(__)_)   SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s     (__)     PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s              UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s              %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else if(strcmp(sysInfo.distro, "Ubuntu") == 0){
        printf("%s\n                %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s                OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s           _    KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s       ---(_)   MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s   _/  ---  \\   CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s  (_) |   |     RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s    \\  --- _/   SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s       ---(_)   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }else{
        printf("%s\n              %s%s@%s%s%s\n", bold, sysInfo.username, reset, bold, sysInfo.hostname, reset);
        printf("%s      ___     OS%s     %s%s\n", bold, reset, sysInfo.os, reset);
        printf("%s     (.. |    KERNEL%s %s%s\n", bold, reset, sysInfo.kernel, reset);
        printf("%s     (<> |    MODEL%s  %s %s%s\n", bold, reset, sysInfo.modelname, sysInfo.modelversion, reset);
        printf("%s    / __  \\   CPU%s    %s%s\n", bold, reset, sysInfo.cpu, reset);
        printf("%s   ( /  \\ /|  RAM%s    %dM / %dM%s\n", bold, reset, sysInfo.ramused, sysInfo.ramtotal, reset);
        printf("%s  _/\\ __)/_)  SHELL%s  %s%s\n", bold, reset, sysInfo.shell, reset);
        printf("%s  \\|/-___\\|/  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s              UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysInfo.day, sysInfo.hour, sysInfo.min, reset);
        printf("%s              %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }
    return 0;
}
