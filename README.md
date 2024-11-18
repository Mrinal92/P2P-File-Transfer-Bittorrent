# Bit Torrent: Peer-to-Peer Group-Based File Sharing System

The **Bit Torrent** project is a peer-to-peer file sharing network that offers functionalities such as sharing files, downloading files, and managing shared files within a group. It is inspired by the BitTorrent system commonly used on the internet.

## Table of Contents

- [Architecture](#architecture)
- [Prerequisites](#prerequisites)
  - [Software Requirements](#software-requirements)
  - [Platform](#platform)
- [Installation](#installation)
- [Usage](#usage)
  - [Tracker Commands](#tracker-commands)
  - [Client Commands](#client-commands)
- [Working](#working)
- [Features](#features)
- [Assumptions](#assumptions)
- [Contact](#contact)

## Architecture

The system architecture comprises multiple clients (users) and a central tracker. The tracker stores metadata about which users have which files, facilitating peer communication for efficient file sharing.

![139272264-5951ea87-0a67-4f53-a49a-c6003274e550](https://github.com/user-attachments/assets/52909976-85ff-4dea-8d7a-8766dda74ebd)


## Prerequisites

### Software Requirements

1. **G++ Compiler**
   - **Installation:**
     ```shell
     sudo apt-get update
     sudo apt-get install g++
     ```

2. **OpenSSL Library**
   - **Installation:**
     ```shell
     sudo apt-get install libssl-dev
     ```

### Platform

- **Operating System:** Linux

## Installation

Follow these steps to build and set up the client and tracker components.

1. **Clone the Repository:**
   ```shell
   git clone https://github.com/yourusername/bit-torrent.git
   cd bit-torrent
   ```

2. **Build the Client:**
   ```shell
   cd client
   make
   ```

3. **Build the Tracker:**
   ```shell
   cd ../tracker
   make
   ```

4. **Return to the Main Directory:**
   ```shell
   cd ..
   ```
   
5. **Usage**
   **Tracker Commands**

    i) ***Run Tracker:***
     To start a tracker, navigate to the tracker directory and execute the following command:
     ```shell
     cd tracker
     ./tracker <TRACKER_INFO_FILE> <TRACKER_NUMBER>
     ```
     Example:
     ./tracker tracker_info.txt 1
     <TRACKER_INFO_FILE>: A file containing IP and port details of all trackers.
     Example tracker_info.txt:
     127.0.0.1
     5353
     127.0.0.1
     50686

    ii) ***Close Tracker:***
    To shut down the tracker, type the following command within the tracker interface:
    ```shell
     quit
    ```

    iii) ***Client Commands***

     Run Client: To start a client, navigate to the client directory and execute:
     ```shell
     cd client
     ./client <IP>:<PORT> <TRACKER_INFO_FILE>
     ```
     
     Example:
     ./client 127.0.0.1:18000 tracker_info.txt

     iv) ***Available Client Commands:***

        a) Create User Account:
           create_user <user_id> <password>

        b) Login:
           login <user_id> <password>

        c) Create Group:

           create_group <group_id>

        d) Join Group:
        
           join_group <group_id>
        
        e) Leave Group:
        
           leave_group <group_id>
        
        f) List Pending Join Requests:
        
           list_requests <group_id>
        
        g) Accept Group Joining Request:
        
           accept_request <group_id> <user_id>
        
        h) List All Groups in Network:
        
           list_groups
        
        i) List All Sharable Files in Group:
        
           list_files <group_id>
        
        j) Upload File:
        
           upload_file <file_path> <group_id>
        
        k) Download File:
        
           download_file <group_id> <file_name> <destination_path>
        
        l) Logout:
        
           logout
        
        m) Show Downloads:
        
           show_downloads
        
        n) Stop Sharing a File:
        
            stop_share <group_id> <file_name>
        
        o) Output Format for Downloads:
        
            Downloading:
            [D] [group_id] filename

            Complete:
            [C] [group_id] filename

**Working**

    ***User Registration and Login:***
        Users create accounts and register with the tracker.
        Authentication is required for login using user credentials.

    ***Group Management:***
        Users can create groups, automatically becoming the group owner.
        Owners can manage group memberships by accepting or rejecting join requests.
        Users can join or leave existing groups.

    ***File Sharing:***
        Users can share files within a group by providing the filename and SHA1 hash.
        Shared files are mapped to the user's IP and port in the tracker.

    ***Downloading Files:***
        Users can download files from the group by fetching peer information from the tracker.
        Files are downloaded in parallel from multiple peers, ensuring efficient and reliable transfers.
        Each file is divided into 512KB pieces, with SHA1 hashes verifying integrity.

    ***Concurrency and Resilience:***
        The system supports multiple simultaneous downloads and uploads.
        At least one tracker remains online to maintain system availability.
        Trackers are synchronized to ensure consistent metadata across the network.

    ***Session Management:***
        Upon login, previously shared files are automatically set to sharing mode.
        Logging out temporarily stops sharing files until the next login.

**Features**

    ***User Management:***
        Account creation and secure authentication.

    ***Group Management:***
        Create, join, leave groups.
        Manage group memberships with admin controls.

    ***File Sharing and Downloading:***
        Share files with SHA1 integrity verification.
        Download files in parallel from multiple peers.
        Custom piece selection algorithm to optimize download efficiency.

    ***Tracker Synchronization:***
        Dual tracker system ensures metadata consistency and system reliability.

    ***Concurrency Support:***
        Handle multiple downloads and uploads simultaneously without performance degradation.

**Assumptions**

    ***Tracker Availability:***
        Only one tracker is implemented, and it should remain online at all times.

    ***Peer Details Persistence:***
        Peer information is not persistent across sessions, even if users log in from different IP addresses.

    ***File Integrity:***
        SHA1 integrity checking may not work correctly for certain binary files, though most files should download correctly.

    ***File Paths:***
        File paths provided during upload and download operations should be absolute.
