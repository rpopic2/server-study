use std::io::{stdin, ErrorKind, Read, Write};
use std::net::TcpStream;
fn main() {
    let mut socket = TcpStream::connect("127.0.0.1:8080").expect("Could not connect to server");
    let mut write_buf = [0u8; 1024];
    socket
        .set_nonblocking(true)
        .expect("Could not set non-blocking");

    let mut read_buf = String::new();
    loop {
        stdin()
            .read_line(&mut read_buf)
            .expect("Could not read from stdin");
        socket
            .write(read_buf.as_bytes())
            .expect("Could not write to socket");
        read_buf.clear();
        println!("1");
        let bytes_read = match socket.read(&mut write_buf) {
            Ok(bytes_read) => bytes_read,
            Err(e) => {
                if e.kind() == ErrorKind::WouldBlock {
                    continue;
                } else {
                    panic!("Encountered IO error: {}", e);
                }
            }
        };
        println!("2");
        match bytes_read {
            0 => break,
            _ => println!("Received: {}", String::from_utf8_lossy(&write_buf[..])),
            //_ => panic!("Received unexpected number of bytes of {}", bytes_read),
        }
    }
}
