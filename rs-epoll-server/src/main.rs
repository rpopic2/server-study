use polling::Poller;
use std::{
    collections::HashMap,
    io::{Read, Write},
    net::TcpListener,
    os::unix::prelude::AsRawFd,
};

fn main() {
    let server_listener = TcpListener::bind("127.0.0.1:8080").expect("Bind failed");
    let server_key = server_listener.as_raw_fd() as usize;

    println!("{}", server_key);
    server_listener
        .set_nonblocking(true)
        .expect("Failed to set non-blocking");

    let poller = Poller::new().expect("Poller creation failed");
    poller
        .add(&server_listener, polling::Event::readable(server_key))
        .expect("Failed to add socket to poller");

    println!("Starting server.");

    let mut sockets = HashMap::new();

    let mut events = Vec::new();
    loop {
        events.clear();
        poller.wait(&mut events, None).expect("Poller wait failed");

        for ev in &events {
            if ev.key == server_key {
                // when new incoming connection arrives, accept it and add it to the poller

                let (client_socket, _) = server_listener.accept().expect("Accept failed");
                let client_fd = client_socket.as_raw_fd() as usize;
                client_socket
                    .set_nonblocking(true)
                    .expect("Failed to set non-blocking");
                poller
                    .add(&client_socket, polling::Event::all(client_fd))
                    .unwrap();
                poller
                    .modify(&server_listener, polling::Event::readable(server_key))
                    .expect("Modify failed");
                println!(
                    "Client connected : {}, fd : {}",
                    client_socket.peer_addr().unwrap(),
                    client_fd
                );
                sockets.insert(client_fd, client_socket);
            } else {
                // when data arrives from a client

                let ev_key = ev.key;
                let client_stream = sockets.get_mut(&ev_key).expect("Fd not found");
                let mut buf = [0u8; 1024];
                match client_stream.read(&mut buf) {
                    Ok(0) => {
                        //if it is a EOF
                        println!("Client {} disconnected", ev_key);
                        poller.delete(&sockets[&ev_key]).expect("Remove failed");
                        sockets.remove(&ev_key);
                        continue;
                    }
                    Ok(_) => {
                        //if it is a message
                        println!(
                            "Response from client {}: {}",
                            ev_key,
                            String::from_utf8_lossy(&buf)
                        );
                        poller
                            .modify(&sockets[&ev_key], polling::Event::all(ev_key))
                            .expect("Modify failed");
                    }
                    Err(e) => {
                        //if it would block
                        if e.kind() == std::io::ErrorKind::WouldBlock {
                            println!("Would block");
                            continue;
                        }
                        //else panic if other error arrives
                        println!("Error reading from client {}: {}", ev_key, e);
                        continue;
                    }
                }
            }
        }
    }
}
