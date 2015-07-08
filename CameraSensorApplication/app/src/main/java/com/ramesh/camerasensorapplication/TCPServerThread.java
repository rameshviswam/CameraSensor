/**
 * Created by rviswana on 7/5/2015.
 */
package com.ramesh.camerasensorapplication;

import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.DataOutputStream;

import java.net.ServerSocket;
import java.net.Socket;

public class TCPServerThread implements Runnable {
    public static final int SERVER_PORT = 1456;

    private ServerSocket server;
    private Socket socket;

    private BufferedReader streamIn;
    private DataOutputStream streamOut;
    //private PrintWriter mWriter;

    @Override
    public void run() {
        try {
            openSocket();
            openStream();
            boolean done = false;
            while (!done) {
                try {
                    // String line = streamIn.readUTF();
                    String line = streamIn.readLine();
                    Log.e("RV........", line);
                    done = line.equals("bye");
                    String newOut = new String(line);
                    Log.e("RV......", newOut);
                    streamOut.writeBytes(newOut);
                    streamOut.writeBytes("\r\n");
                } catch (IOException ioe) {
                    Log.e("RV..EXCEPT.....", ioe.getLocalizedMessage());
                    done = true;
                }
            }
            closeStream();
        } catch (IOException ioe) {
            Log.e("RV..EXCEPT.....", ioe.getLocalizedMessage());
        }
        finally {
            try {
                closeSocket();
            } catch(IOException ioe) {
                Log.e("RV..EXCEPT.....", ioe.getLocalizedMessage());
            }
        }
    }

    private void openSocket() throws IOException {
        System.out.println("Binding to port " + SERVER_PORT + ", please wait  ...");
        server = new ServerSocket(SERVER_PORT);
        Log.e("RV..............", "Server started: " + server);

        Log.e("RV............", "Waiting for a client ...");
        socket = server.accept();
        Log.e("RV............", ("Client accepted: " + socket));
    }

    private void openStream() throws IOException {
        //streamIn = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
        streamIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        streamOut = new DataOutputStream(socket.getOutputStream());
    }

    private void closeSocket() throws IOException {
        if (socket != null)
            socket.close();
    }

    private void closeStream() throws IOException {
        if (streamIn != null)
            streamIn.close();

        if (streamOut != null)
            streamOut.close();
    }
}