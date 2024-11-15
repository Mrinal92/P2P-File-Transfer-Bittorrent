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

![Architecture Diagram](architecture_diagram.png) *(Include an actual diagram if available)*

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

    Build the Client:

cd client
make

Build the Tracker:

cd ../tracker
make

Return to the Main Directory:

    cd ..

Usage
Tracker Commands

Run Tracker:

To start a tracker, navigate to the tracker directory and execute the following command:

cd tracker
./tracker <TRACKER_INFO_FILE> <TRACKER_NUMBER>

Example:

./tracker tracker_info.txt 1

    <TRACKER_INFO_FILE>: A file containing IP and port details of all trackers.

    Example tracker_info.txt:

    127.0.0.1
    5353
    127.0.0.1
    50686

Close Tracker:

To gracefully shut down the tracker, type the following command within the tracker interface:

quit

Client Commands

Run Client:

To start a client, navigate to the client directory and execute:

cd client
./client <IP>:<PORT> <TRACKER_INFO_FILE>

Example:

./client 127.0.0.1:18000 tracker_info.txt

Available Client Commands:

    Create User Account:

create_user <user_id> <password>

Login:

login <user_id> <password>

Create Group:

create_group <group_id>

Join Group:

join_group <group_id>

Leave Group:

leave_group <group_id>

List Pending Join Requests:

list_requests <group_id>

Accept Group Joining Request:

accept_request <group_id> <user_id>

List All Groups in Network:

list_groups

List All Sharable Files in Group:

list_files <group_id>

Upload File:

upload_file <file_path> <group_id>

Download File:

download_file <group_id> <file_name> <destination_path>

Logout:

logout

Show Downloads:

show_downloads

Stop Sharing a File:

    stop_share <group_id> <file_name>

Output Format for Downloads:

    Downloading:

[D] [group_id] filename

Complete:

    [C] [group_id] filename

Working

    User Registration and Login:
        Users create accounts and register with the tracker.
        Authentication is required for login using user credentials.

    Group Management:
        Users can create groups, automatically becoming the group owner.
        Owners can manage group memberships by accepting or rejecting join requests.
        Users can join or leave existing groups.

    File Sharing:
        Users can share files within a group by providing the filename and SHA1 hash.
        Shared files are mapped to the user's IP and port in the tracker.

    Downloading Files:
        Users can download files from the group by fetching peer information from the tracker.
        Files are downloaded in parallel from multiple peers, ensuring efficient and reliable transfers.
        Each file is divided into 512KB pieces, with SHA1 hashes verifying integrity.

    Concurrency and Resilience:
        The system supports multiple simultaneous downloads and uploads.
        At least one tracker remains online to maintain system availability.
        Trackers are synchronized to ensure consistent metadata across the network.

    Session Management:
        Upon login, previously shared files are automatically set to sharing mode.
        Logging out temporarily stops sharing files until the next login.

Features

    User Management:
        Account creation and secure authentication.

    Group Management:
        Create, join, leave groups.
        Manage group memberships with admin controls.

    File Sharing and Downloading:
        Share files with SHA1 integrity verification.
        Download files in parallel from multiple peers.
        Custom piece selection algorithm to optimize download efficiency.

    Tracker Synchronization:
        Dual tracker system ensures metadata consistency and system reliability.

    Concurrency Support:
        Handle multiple downloads and uploads simultaneously without performance degradation.

Assumptions

    Tracker Availability:
        Only one tracker is implemented, and it should remain online at all times.

    Peer Details Persistence:
        Peer information is not persistent across sessions, even if users log in from different IP addresses.

    File Integrity:
        SHA1 integrity checking may not work correctly for certain binary files, though most files should download correctly.

    File Paths:
        File paths provided during upload and download operations should be absolute.
