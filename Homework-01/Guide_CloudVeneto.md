# How to set up a Linux virtual machine on CloudVeneto

This guide describes how to set up a functioning working enviroment on CloudVeneto.

## 1. Apply for an account

Go to https://cloud-areapd.pd.infn.it/dashboard and log in using with a UniPD or INFN account.

Fill the user registration form and select and existing project, for example the `SC4PS-PhD ` project.

Once your request has been approved you will recive and email with tha credential to the **Cloud gate machine**.

## 2. Access the Cloud Gate

With your credential you should be able to access the server Gate:

```
ssh user@gate.cloudveneto.it
```

After the first login you will be asked to change the password.

From inside the Gate, you will be able to connect to the virtual machine. For the moment you can quit with:

```
exit
```
## 3. Create a keypair

With your account you can interact with the Cloud via the **dashboard** web interface https://cloudveneto.ict.unipd.it/dashboard/

The first thing you should do is to create a **keypair**. This is a secret key which will allow you to interact with your virtual machines once they are created.

To create the key follow these steps:

- Open `Compute` tab
- Select `Key pairs`
- Select `Create Key pair`
- Give the pair a name, e.g. `my_key`
- Select `SSH key` on Key type 

The file `my_key.pem` will then be downloaded. You should move the file to a different location `path/my.key.pem`

You should also make the key private with:

```
chmod 600 path/my_key.pem 
```

## 4. Create a virtual machine on CloudVeneto

Now taht you have a key, you can create a **virtual machine**. 

From the **dashboard**, under the appropaite project follow these steps:

- Go to `Compute -> Instances`
- Click on `Launch Instance`

A new window will appear asking for the settings of the virtual machine.

- Under `Details` give an `Instance Name`
- Under `Source` select an `image`, e.g. `AlmaLinux-9-2026-02-17`
- Under `Flavour` you can select a configration for the CPU, RAM, and Storage allocated to the virtual machine
- Uder `Key Pair` select the previously created key.


## 5. Access the virtual machine

In the **dashboard** under `Compute -> Instances` you should see your virtual machine, with an associated **IP Address**. 

To accesss the machine, you need to connect with `ssh` to that **IP Address** from the Cloud Gate, and using the key. 

``` 
ssh -J userE@gate.cloudveneto.it -i path/my_key.pem almalinux@IP_Address
```
Note: `almalinux` is the default user for for AlmaLinux images, it is different for Ubuntu and CentOS machines


## 6. Installing the C compiler on the VM

The first time you access the machine you should update the system and package list:

```
sudo dnf check-update
sudo dnf upgrade -y
```

Now you can install the `Development Tools` which includes basic tools required for C/C++ development, such as the compiler `gcc` and `make`.

```
sudo dnf groupinstall -y "Development Tools"
```

Verify the installation of the compiler:

```
gcc --version
```

## 7. Create and compile a C program

Create a `Hello.c` file using a text editor like `nano` or `gedit`, which you should install like:

```
sudo dnf install gedit
```

Paste the following lines inside the file:

```
#include <stdio.h>

int main() {
 
    printf("O brave new world that has such people in it! \n");

    return 0;
}
```

You can compile with:

```
gcc -o output hello.c
```

The flag `-o` specifies the name of the output program. 

Execute it with:
```
./output
```


