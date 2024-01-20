# Bit Torrent: Peer-to-Peer Group-Based File Sharing System

The Mini-torrent project is a peer-to-peer file sharing network, offering functionalities such as sharing files, downloading files, and managing shared files within a group. It resembles the BitTorrent system found on the internet.

## Architecture

The architecture consists of multiple clients (users) and a central tracker that stores metadata about which users have which files, creating a file-user mapping.



## Prerequisites

### Software Requirements

1. G++ compiler
   - To install G++: `sudo apt-get install g++`

2. OpenSSL library
   - To install the OpenSSL library: `sudo apt-get install openssl`

### Platform

- Linux

## Installation

1. Navigate to the client directory:
   ```shell
   cd client

    Build the client:

    shell

make

Navigate to the tracker directory:

shell

cd ../tracker

Build the tracker:

shell

make

Return to the main directory:

shell

    cd ..

Usage
Tracker

Run Tracker:

To run the tracker, use the following command in the tracker directory:

shell

cd tracker
./tracker <TRACKER INFO FILE> <TRACKER NUMBER>

For example:

shell

./tracker tracker_info.txt 1

The <TRACKER INFO FILE> contains the IP and port details of all the trackers. Here's an example:

yaml

127.0.0.1
5353
127.0.0.1
50686

Close Tracker:

To close the tracker, simply type:

shell

quit

Client

Run Client:

To run a client, navigate to the client directory and use the following command:

shell

cd client
./client <IP>:<PORT> <TRACKER INFO FILE>

For example:

shell

./client 127.0.0.1:18000 tracker_info.txt

Available Client Commands:

    Create user account:

    shell

create_user <user_id> <password>

Login:

shell

login <user_id> <password>

Create Group:

shell

create_group <group_id>

Join Group:

shell

join_group <group_id>

Leave Group:

shell

leave_group <group_id>

List pending requests:

shell

list_requests <group_id>

Accept Group Joining Request:

shell

accept_request <group_id> <user_id>

List All Group In Network:

shell

list_groups

List All Sharable Files In Group:

shell

list_files <group_id>

Upload File:

shell

upload_file <file_path> <group_id>

Download File:

shell

download_file <group_id> <file_name> <destination_path>

Logout:

shell

logout

Show Downloads:

shell

show_downloads

Stop Sharing:

shell

    stop_share <group_id> <file_name>

Working

    Users create accounts and register with the tracker.

    Users log in using their credentials.

    The tracker maintains information about clients and the files they share to facilitate communication between peers.

    Users can create groups and become group admins.

    Users can fetch a list of all groups on the server.

    Users can join/leave groups.

    Group admins can accept group join requests.

    Users can share files across groups, including filename and SHA1 hash.

    Download files by retrieving peer information from the tracker.

    Show downloads and stop sharing files.

    Upon login, previously shared files are automatically set to the sharing mode.

Assumptions

    Only one tracker is implemented, and it should always be online.

    Peer details are not persistent across sessions, even if logging in from different IP addresses.

    SHA1 integrity checking may not work correctly for binary files, although most files would likely download correctly.

    File paths should be absolute.
