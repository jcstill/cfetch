/*
 * cfetch: A minimal system information tool for Linux-based operating systems.
 * Copyright (c) 2020 Clint
 * This programme is provided under the GPL-3.0 License. See LICENSE for more details.
 */





#include <stdio.h>
#include <string.h>

// Colours and text customisation.
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


// System information variables.
struct sysinfo {
    char username[25], hostname[50], os[50], kernel[50], modelname[50], modelversion[50], cpu[50], shell[8];
    int ramused, ramtotal;
    int apt, dnf, emerge, flatpak, kiss, pacman, rpm, snap, yay, yum, zypper;
    int day, hour, min, sec;
} sysinfo;

// Gets the current user.
// This will get the current user and the hostname.
void getuser() {
    FILE *username = popen("echo $USER", "r");
    FILE *hostname = popen("cat /proc/sys/kernel/hostname", "r");
    fscanf(username, "%s", sysinfo.username);
    fscanf(hostname, "%s", sysinfo.hostname);
    fclose(username);
    fclose(hostname);
}

// Gets the distro name.
// This will get the name of the distro.
void getos() {
    FILE *os = popen("if [ $(command -v lsb_release) ];then lsb_release -ds;else cat /etc/*-release | grep 'PRETTY_NAME=' | cut -d '\"' -f2;fi 2>/dev/null", "r");
    fscanf(os, "%[^\n]s", sysinfo.os);
    fclose(os);
}

// Gets the kernel.
// This will get the kernel-release, kernel-name, and the hardware machin name.
void getkernel() {
    FILE *kernel = popen("uname -rsm", "r");
    fscanf(kernel, "%[^\n]s", sysinfo.kernel);
    fclose(kernel);
}

// Gets the model information.
// This will get the computer's product name and the product version.
void getmodel() {
    FILE *modelname = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    FILE *modelversion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
    fscanf(modelname, "%[^\n]s", sysinfo.modelname);
    fscanf(modelversion, "%s", sysinfo.modelversion);
    fclose(modelname);
    fclose(modelversion);
}

// Gets the shell information.
void getshell() {
    FILE *shell = popen("basename $SHELL", "r");
    fscanf(shell, "%s", sysinfo.shell);
    fclose(shell);
}

// Gets the packages.
// Supports apt, dnf, emerge, flatpak, kiss, pacman, rpm, snap, yay, yum, zypper.
void getpackages() {
    FILE *apt = popen("dpkg-query -f '${binary:Package}\n' -W 2>/dev/null | wc -l", "r");
    FILE *dnf = popen("dnf list installed 2>/dev/null | wc -l", "r");
    FILE *emerge = popen("qlist -I 2>/dev/null | wc -l", "r");
    FILE *flatpak = popen("flatpak list 2>/dev/null | wc -l", "r");
    FILE *kiss = popen("kiss list 2>/dev/null | wc -l", "r");
    FILE *pacman = popen("pacman -Q 2>/dev/null | wc -l", "r");
    FILE *rpm = popen("rpm -qa --last 2>/dev/null | wc -l", "r");
    FILE *snap = popen("snap list 2>/dev/null | wc -l", "r");
    FILE *yay = popen("yay -Q 2>/dev/null | wc -l", "r");
    FILE *yum = popen("yum list installed 2>/dev/null | wc -l", "r");
    FILE *zypper = popen("zypper se 2>/dev/null | wc -l", "r");

    fscanf(apt, "%d", &sysinfo.apt);
    fscanf(dnf, "%d", &sysinfo.dnf);
    fscanf(emerge, "%d", &sysinfo.emerge);
    fscanf(flatpak, "%d", &sysinfo.flatpak);
    fscanf(kiss, "%d", &sysinfo.kiss);
    fscanf(pacman, "%d", &sysinfo.pacman);
    fscanf(rpm, "%d", &sysinfo.rpm);
    fscanf(snap, "%d", &sysinfo.snap);
    fscanf(yay, "%d", &sysinfo.yay);
    fscanf(yum, "%d", &sysinfo.yum);
    fscanf(zypper, "%d", &sysinfo.zypper);
    fclose(apt);
    fclose(dnf);
    fclose(emerge);
    fclose(flatpak);
    fclose(kiss);
    fclose(pacman);
    fclose(rpm);
    fclose(snap);
    fclose(yay);
    fclose(yum);
    fclose(zypper);
}

// Print the correct packages and names.
void printpkgs(){
    int comma=0;
    if (sysinfo.apt != 0) {
        if (comma == 1) printf(", ");
        printf("apt: %d", sysinfo.apt);
        comma = 1;
    }
    if (sysinfo.dnf != 0) {
        if(comma == 1) printf(", ");
        printf("dnf: %d", sysinfo.dnf);
        comma = 1;
    }
    if (sysinfo.emerge != 0) {
        if (comma == 1) printf(", ");
        printf("emerge: %d", sysinfo.emerge);
        comma = 1;
    }
    if (sysinfo.flatpak != 0) {
        if (comma == 1) printf(", ");
        printf("flatpak: %d", sysinfo.flatpak);
        comma = 1;
    }
    if (sysinfo.kiss != 0) {
        if (comma == 1) printf(", ");
        printf("kiss: %d", sysinfo.kiss);
        comma = 1;
    }
    if (sysinfo.pacman != 0) {
        if (comma == 1) printf(", ");
        printf("pacman: %d", sysinfo.pacman);
        comma = 1;
    }
    if (sysinfo.rpm != 0) {
        if (comma == 1) printf(", ");
        printf("rpm: %d", sysinfo.rpm);
        comma = 1;
    }
    if (sysinfo.snap != 0) {
        if (comma == 1) printf(", ");
        printf("snap: %d", sysinfo.snap);
        comma = 1;
    }
    if (sysinfo.yay != 0) {
        if (comma == 1) printf(", ");
        printf("yay: %d", sysinfo.yay);
        comma = 1;
    }
    if (sysinfo.yum != 0) {
        if (comma == 1) printf(", ");
        printf("yum: %d", sysinfo.yum);
        comma=1;
    }
    if (sysinfo.zypper != 0) {
        if (comma == 1) printf(", ");
        printf("zypper: %d", sysinfo.zypper);
        comma = 1;
    }

    printf("%s\n", reset);
}

// Get the CPU information.
// lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//
// sed -r 's/Model name:\\s{1,}// -> This will remove the 'Model name:', only the CPU will be printed.
void getcpu() {
    FILE *cpu = popen("lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//'", "r");
    fscanf(cpu, "%[^\n]s", sysinfo.cpu);
    fclose(cpu);
}

// Get the RAM information.
// This will show the used RAM / total RAM.
void getram() {
    FILE *ramused = popen("vmstat -s -S M | grep ' used memory'", "r");
    FILE *ramtotal = popen("vmstat -s -S M | grep ' total memory'", "r");
    fscanf(ramused, "%d", &sysinfo.ramused);
    fscanf(ramtotal, "%d", &sysinfo.ramtotal);
    fclose(ramused);
    fclose(ramtotal);
}

// Get the uptime information.
// This will show how long the computer is running: DDd, HHh, MMm.
void getuptime() {
    FILE *uptime = fopen("/proc/uptime", "r");
    fscanf(uptime, "%d", &sysinfo.sec);
    fclose(uptime);

    sysinfo.day = (sysinfo.sec/60/60/24);
    sysinfo.hour = (sysinfo.sec/60/60%24);
    sysinfo.min = (sysinfo.sec/60%60);
}

// Call all functions that gets the system information.
void getsysinfo() {
    getuser();
    getos();
    getmodel();
    getkernel();
    getshell();
    getpackages();
    getcpu();
    getram();
    getuptime();
}

int main() {
    getsysinfo();

    // Do not change these.
    // user, os, kernel, model, cpu, ram, shell, pkgs, uptime, palette colours.
    if (strstr(sysinfo.os, "Arch") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        /\\        OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       /  \\       KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s      /\\   \\      MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     /  __  \\     CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s    /  (  )  \\    RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   / __|  |__ \\   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /.\\`      \\`.\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Debian") != NULL) {
        printf("%s                %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s     ,---._     KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   /\\`  __ \\\\   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  |   /    |    CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s  |   \\`.__.\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   \\            SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s    \\`-,_       PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Devuan") != NULL) {
        printf("%s                   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s  -.,              OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s     \\`'-.,        KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s          \\`':.    MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s             ::    CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s        __--\\`:\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   _,--\\` _.-\\`    SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  :_,--\\`\\`        PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                   UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Elementary") != NULL) {
        printf("%s               %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s               OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s    _______    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   /  ___  \\   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  /  |  /  /\\  CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s  |__\\ /  / |  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  \\   /__/  /  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s   \\_______/   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s               UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s               %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Fedora") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        _____     OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       /   __)\\   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s       |  /  \\ \\  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    ___|  |__/ /  CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s   / (_    _)_/   RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  / /  |  |       SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  \\ \\__/  |       PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s   \\(_____/       UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Gentoo") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s    .-----.       OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  .\\`    _  \\`.   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  \\`.   ()   \\`.  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    \\`.        /  CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s   .\\`       .\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  /       .\\`     SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  \\____.-\\`       PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Lite") != NULL) {
        printf("%s          %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s     /\\   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s    /  \\  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   / / /  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  > / /   CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s  \\ \\ \\   RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   \\_\\_\\  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s      \\   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s          UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s          %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Mint") != NULL) {
        printf("%s                   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s   _____________   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  |_            \\  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s    |  |  ___   |  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    |  | | | |  |  CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s    |  | | | |  |  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s    |   \\____/  |  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s    \\__________/   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                   UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Manjaro") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s  ||||||||| ||||  OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  ||||||||| ||||  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  ||||      ||||  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  |||| |||| ||||  CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s  |||| |||| ||||  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  |||| |||| ||||  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  |||| |||| ||||  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "MX") != NULL) {
        printf("%s                %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s      \\\\  /     OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       \\\\/      KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s        \\\\      MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     /\\/ \\\\     CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s    /  \\  /\\    RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   /    \\/  \\   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /__________\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "SUSE") != NULL) {
        printf("%s                 %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s      _______    OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  -___|   __ \\   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s         / .\\ \\  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s         \\__/ |  CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s       _______|  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s       \\_______  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  --__________/  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                 UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                 %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Pop") != NULL) {
        printf("%s                     %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s  ______             OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  \\   _ \\        __  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   \\ \\ \\ \\      / /  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    \\ \\_\\ \\    / /   CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s     \\  ___\\  /_/    RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s      \\ \\    _       SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s     __\\_\\__(_)_     PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s    (___________)    UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                     %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Pure") != NULL) {
        printf("%s                   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s   _____________   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  |  _________  |  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  | |         | |  CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s  | |         | |  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  | |_________| |  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  |_____________|  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                   UP TIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Raspbian") != NULL) {
        printf("%s              %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s              OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s    __  __    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   (_\\)(/_)   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s   (_(__)_)   CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s  (_(_)(_)_)  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   (_(__)_)   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s     (__)     PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s              UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s              %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Ubuntu") != NULL) {
        printf("%s                %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s           _    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s       ---(_)   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s   _/  ---  \\   CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s  (_) |   |     RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s    \\  --- _/   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s       ---(_)   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else {
        printf("%s              %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s      ___     OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s     (.. |    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s     (<> |    MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    / __  \\   CPU%s    %s%s\n", bold, reset, sysinfo.cpu, reset);
        printf("%s   ( /  \\ /|  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  _/\\ __)/_)  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  \\|/-___\\|/  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s              UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s              %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }

    return 0;
}
