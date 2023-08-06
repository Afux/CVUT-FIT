using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Text.RegularExpressions;

namespace ConsoleApp1
{
    public class Key
    {

        public string KeyId;
        public string ServerKey;
        public string ClientKey;

        public Key(string KeyId, string ServerKey, string ClientKey)
        {
            this.KeyId = KeyId;
            this.ServerKey = ServerKey;
            this.ClientKey = ClientKey;
        }
    }

    //All messages
    public class Messeges
    {

        protected string SERVER_MOVE = "102 MOVE\a\b";
        protected string SERVER_TURN_LEFT = "103 TURN LEFT\a\b";
        protected string SERVER_TURN_RIGHT = "104 TURN RIGHT\a\b";
        protected string SERVER_PICK_UP = "105 GET MESSAGE\a\b";
        protected string SERVER_LOGOUT = "106 LOGOUT\a\b";
        protected string SERVER_KEY_REQUEST = "107 KEY REQUEST\a\b";
        protected string SERVER_OK = "200 OK\a\b";
        protected string SERVER_LOGIN_FAILED = "300 LOGIN FAILED\a\b";
        protected string SERVER_SYNTAX_ERROR = "301 SYNTAX ERROR\a\b";
        protected string SERVER_LOGIC_ERROR = "302 LOGIC ERROR\a\b";
        protected string SERVER_KEY_OUT_OF_RANGE_ERROR = "303 KEY OUT OF RANGE\a\b";

        protected int CLIENT_USERNAME_Lenght = 18;
        protected int CLIENT_KEY_ID_Lenght = 3;
        protected int CLIENT_CONFIRMATION_Lenght = 5;
        protected int CLIENT_OK_Lenght = 10;
        protected int CLIENT_RECHARGING_Lenght = 10;
        protected int CLIENT_FULL_POWER_Lenght = 10;
        protected int CLIENT_MESSAGE_Lenght = 98;

    }


    //Helper custom funcs
    public class CheckFunc:Messeges
    {
        public TcpClient client;
        protected ServerObject server;
        public int posx, posy;
        public List<string> splitted = new List<string>();
        string[] words;
        public bool Needcharged = false;
       

        public string userName;
        public int userHash = 0;
        public string Key1;
        public string ServerKey;
        public string ClientKey;
        public string klientHash;


        //Spliting long message
        protected void SplitThemOut(string mes)
        {
            words = mes.Split("\a\b");

            foreach (var word in words)
            {
                if (!word.Equals(""))
                    splitted.Add(word + "\a\b");
            }
        }

        //Checking if Key exists
        protected bool check_Key(string key)
        {
            key = pure_datas(key);
           int tmp = Convert.ToInt32(key);
           switch (tmp)
            {
                case 0:
                    return true;
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
                case 4:
                    return true;
                default:
                    return false;
            }
        
        }

        //Geting message from client
        protected string GetMessage(NetworkStream Stream)
        {
            byte[] data = new byte[256];
            StringBuilder builder = new StringBuilder();
            int bytes = 0;
            do
            {
                bytes = Stream.Read(data, 0, data.Length);
                builder.Append(Encoding.UTF8.GetString(data, 0, bytes));
            }
            while (Stream.DataAvailable);

            return builder.ToString();
        }

        //Reading message until postfix
        protected void GetSplittedMessage(string message, string tmp,NetworkStream Stream)
        {
            do
            {

                message = GetMessage(Stream);
                tmp += message;
                message = tmp;

            }
            while (!message.EndsWith("\a\b"));
            tmp = null;
        }

        //Converting usingname to ASCII code and geting hash
        protected void Find_Key_hash(List<Key> keys, string Id)
        {

            Key1 = pure_datas(Key1);
            
            for (int i = 0; i < keys.Count; i++)
            {

                if (Key1.Equals(keys[i].KeyId))
                {

                    Key1 = keys[i].KeyId;
                    ClientKey = keys[i].ClientKey;
                    ServerKey = keys[i].ServerKey;

                    byte[] ASCIIValues = Encoding.ASCII.GetBytes(pure_datas(userName));
                    foreach (byte b in ASCIIValues)
                    {
                        userHash += b;

                    }
                    userHash = (userHash * 1000) % 65536;
                    //SERVER_CONFIRMATION MSG
                    server.SentMessage(Convert.ToString(GetHash(Key1, ServerKey, ClientKey, userHash))+"\a\b", Id);

                    break;
                }
            }
        }
        //HashFormula
        protected int GetHash(string KID, string SK, string CK, int hash)
        {
            int tmp;
            int tmp1 = Convert.ToInt32(KID);
            int tmp2 = Convert.ToInt32(SK);
            int tmp3 = Convert.ToInt32(CK);
            tmp = (hash + tmp2) % 65536;
            return tmp;

        }

        //Checking client message lenght
        protected bool check_len(string data,int stage)
        {
            data = pure_datas(data);
            int tmp = data.Length;
            bool state;
            switch (stage)
            {
                case 0:
                    state= tmp <= CLIENT_USERNAME_Lenght;
                    break;
                   
                case 1:
                    state = tmp <= CLIENT_KEY_ID_Lenght;
                    break;

                case 2:
                    state = tmp <= CLIENT_CONFIRMATION_Lenght;
                    break;

                case 3:
                    state = tmp <= CLIENT_OK_Lenght;
                    break;

                case 4:
                    state = tmp <= CLIENT_USERNAME_Lenght;
                    break;
                case 5:
                    state = tmp <= CLIENT_MESSAGE_Lenght;
                    break;

                default:
                    state = false;
                    break;


            }
            return state;

        }
        //Geting data without /a/b
        protected string pure_datas(string data)
        {
        
           string pure_data = data.Substring(0, data.Length - 2);
            return pure_data;
        }
        //Geting position without /a/b
        protected string pure_position(string data)
        {

            string pure_pos = data.Remove(0, 3);
            pure_pos = pure_datas(pure_pos);
            return pure_pos;
        }
        //Defining message postfix
        protected string postfix(string data)
        {
        
            char tmp3 = data[data.Length - 2];
            char tmp4 = data[data.Length - 1];
            string tmp7 = Convert.ToString(tmp3);
            string tmp8 = Convert.ToString(tmp4);

             string tmp =  tmp7 + tmp8;
             return tmp;
            
        }

        //Checking if syntax is correct
        public bool check_Syntax(string data, int stage,string id,NetworkStream Stream)
        {
            int check_syntax_int;
            
            if (postfix(data) != "\a\b")
                return false;


            data = pure_datas(data);
            switch (stage)
            {
                case 1:
                    if (!data.EndsWith(' '))
                        return int.TryParse(data,out check_syntax_int);
                    return false;
                   
                case 2:
                    if (!data.EndsWith(' '))
                        return int.TryParse(data, out check_syntax_int);
                    else
                        return false;
                   
                case 3:
                    return !data.EndsWith(' ');
                      
                case 100:
                default:
                 
                    return true;
            }
           

        }
        //setting integer from OK_message
        public void getPosition(string data,string id)
        {
            int S;
            string[] tmp = data.Split(' ');
            if(!int.TryParse(tmp[0],out S)||!int.TryParse(tmp[1],out S)||tmp[1].Length==0||tmp[0].Length==0)
            {
                server.SentMessage(SERVER_SYNTAX_ERROR, id);
                throw new Exception(SERVER_SYNTAX_ERROR);
            }
                
            posx = Convert.ToInt32(tmp[0]);
            posy = Convert.ToInt32(tmp[1]);
        }

    }

    public class ClientObject : CheckFunc
    {

        List<Key> keys = new List<Key>();
        public void AddKeys()
        {

            keys.Add(new Key("0", "23019", "32037"));
            keys.Add(new Key("1", "32037", "29295"));
            keys.Add(new Key("2", "18789", "13603"));
            keys.Add(new Key("3", "16443", "29533"));
            keys.Add(new Key("4", "18189", "21952"));

        }
        protected internal string Id { get; private set; }
        protected internal NetworkStream Stream { get; private set; }
        
        int counter = 0;
        int stage = 0;
        int lives;
        int prevX = 0;
        int prevY = 0;
        int splitedCount = 0;
        string currDIST;
        string wantedDIST;
        string detect = "gg";
        bool pickup = false;
        bool wasMove;
        bool long_mess = false;
       

        //Logic for turning to the right diraction
        public void Move_Logic(string curr,string wanted,int x, int y)
        {
            switch (wanted)
            {
                case "up":
                    switch (curr)
                    {
                        case "left":
                            server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            currDIST = "up";
                            wasMove = false;
                            break;
                        case "right":
                            server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            currDIST = "up";
                            wasMove = false;
                            break;
                        case "up":
                            server.SentMessage(SERVER_MOVE, this.Id);
                            wasMove = true;
                            break;
                        case "down":
                            server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            currDIST = "left";
                            wasMove = false;
                            break;
                    }

                    break;
                case "down":
                    switch (curr)
                    {
                        case "left":
                            server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            currDIST = "down";
                            wasMove = false;
                            break;
                        case "right":
                            server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            currDIST = "down";
                            wasMove = false;
                            break;
                        case "up":
                            server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            currDIST = "left";
                            wasMove = false;
                            break;
                        case "down":
                            server.SentMessage(SERVER_MOVE, this.Id);
                            wasMove = true;
                            break;
                    }

                    break;
                case "left":
                    switch (curr)
                    {
                        case "left":
                            server.SentMessage(SERVER_MOVE, this.Id);
                            wasMove = true;
                            break;
                        case "right":
                            server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            currDIST = "down";
                            wasMove = false;
                            break;
                        case "up":
                            server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            currDIST = "left";
                            wasMove = false;
                            break;
                        case "down":
                            server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            currDIST = "left";
                            wasMove = false;
                            break;
                    }
                    break;
                case "right":
                    switch (curr)
                    {
                        case "left":
                            server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            currDIST = "down";
                            wasMove = false;
                            break;
                        case "right":
                            server.SentMessage(SERVER_MOVE, this.Id);
                            wasMove = true;
                            break;
                        case "up":
                            server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            currDIST = "right";
                            wasMove = false;
                            break;
                        case "down":
                            server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            currDIST = "right";
                            wasMove = false;
                            break;
                    }
                    break;
                case "err":
                    switch (curr) 
                    {
                        /*case "1":                   
                            server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            detect = "2";
                            break;*/
                        case "2":
                            server.SentMessage(SERVER_MOVE, this.Id);
                            detect = "3";
                          
                            break;
                        case "3":
                            //server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            if (idk == 1)
                            {
                                server.SentMessage(SERVER_TURN_LEFT, this.Id);
                                currDIST = "up";
                            }
                            else if (idk == 2 )
                            {
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                                currDIST = "right";
                            }
                            else if (idk == 3)
                            {
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                                currDIST = "down";
                            }
                            else if (idk == 4)
                            {
                                server.SentMessage(SERVER_TURN_LEFT, this.Id);
                                currDIST = "left";
                            }
                            else if (idk == 5)
                            {
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                                currDIST = "up";
                            }
                            else if (idk == 6)
                            {
                                server.SentMessage(SERVER_TURN_LEFT, this.Id);
                                currDIST = "left";
                            }

                            else if (idk == 7)
                            {
                                server.SentMessage(SERVER_TURN_LEFT, this.Id);
                                currDIST = "down";
                            }
                            else if (idk == 8)
                            {
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                                currDIST = "right";
                            }
                            else
                             server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            detect = "4";
                            
                            break;


                        case "4":
                            server.SentMessage(SERVER_MOVE, this.Id);
                            detect = "5";

                            break;
                        case "5":
                          
                           
                                server.SentMessage(SERVER_MOVE, this.Id);
                                lives--;
                                detect = "gg";
                                prevX = x;
                                prevY = y;
                                wasMove = true;
                            idk = 0;
                           
                            break;
                        
                        case "gg":
                            // server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            if (x < 0 && wantedDIST == "up")
                            {
                                idk = 1;
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            }
                            else if (x < 0 && wantedDIST == "right")
                            {
                                idk = 2;
                                server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            }
                            else if (x < 0 && wantedDIST == "down")
                            {

                                idk = 3;
                                server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            }
                            else if (x < 0 && wantedDIST == "left")
                            {
                                idk = 4;
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            }
                            else if (x > 0 && wantedDIST == "up")
                            {
                                idk = 5;
                                server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            }

                            else if (x > 0 && wantedDIST == "left")
                            {

                                idk = 6;
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            }
                            else if (x > 0 && wantedDIST == "down")
                            {
                                idk = 7;
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);
                            }

                            else if (x > 0 && wantedDIST == "right")
                            {
                                idk = 8;
                                server.SentMessage(SERVER_TURN_LEFT, this.Id);
                            }
                            else
                                server.SentMessage(SERVER_TURN_RIGHT, this.Id);


                            detect = "2";
                            break;
                    }
                    break;


            }
        }

        int idk = 0;
        //Func for moving and deciding where should robot go
        public void Move(int x, int y)
        {
            bool found = false;
            bool stop = false;

            
            if ((x == prevX && y == prevY && wasMove) || !detect.Equals("gg"))
            {
                // prevX = x;
                //prevY = y;
                if (x == 0 && y == 0)
                {
                    pickup = true;
                    stop = true;
                    return;
                }

                if (lives <= -1)
                {
                    throw new Exception("LOL");
                }

              
                Move_Logic(detect, "err",x,y);

                prevX = x;
                prevY = y;
               

                return;

            }

            if (prevX == 0 && prevY == 0)
            {
                prevX = x;
                prevY = y;
                
                server.SentMessage(SERVER_MOVE, this.Id);
                wasMove = true;
              
                return;
            }
            if (counter == 0)
            {
                if (x > prevX)
                    currDIST = "right";
                 if (x < prevX)
                    currDIST = "left";
                if (y > prevY)
                    currDIST = "up";
                 if (y < prevY)
                    currDIST = "down";
                
                counter++;
            }

            if (x == 0 && y == 0)
            {
                pickup = true;
                stop = true;
            }





            if (y != 0  && found == false)
                {

                
                    
                    if ((y == prevY && y < 0) && found == false)
                    {
                        prevY = y;
                        prevX = x;
                        wantedDIST = "up";
                        found = true;
                    }
                    else if ((y == prevY && y > 0) && found == false)
                    {
                        prevY = y;
                        prevX = x;

                        wantedDIST = "down";
                        found = true;
                    }


                    if ((y < prevY) && found == false)
                    {

                        prevX = x;
                        prevY = y;
                        // currDIST = "down";
                        if (y < 0)
                            wantedDIST = "up";
                        else
                            wantedDIST = "down";
                        found = true;




                    }

                    if ((y > prevY) && found == false)
                    {
                        prevX = x;
                        prevY = y;
                        //currDIST = "up";
                        if (y > 0)
                            wantedDIST = "down";
                        else
                            wantedDIST = "up";
                        found = true;
                    }
                }
            
            
            
                if (x != 0 && found == false)
                {
                
                    if ((x == prevX && x < 0) && found == false)
                    {
                        prevY = y;
                        prevX = x;

                        wantedDIST = "right";
                        found = true;
                    }
                    else if ((x == prevX && x > 0) && found == false)
                    {
                        prevY = y;
                        prevX = x;

                        wantedDIST = "left";
                        found = true;
                    }



                    if ((x < prevX) && found == false)
                    {
                        prevX = x;
                        prevY = y;
                        //  currDIST = "left";
                        if (x < 0)
                            wantedDIST = "right";
                        else
                            wantedDIST = "left";
                        found = true;

                    }
                    if ((x > prevX) && found == false)
                    {
                        prevX = x;
                        prevY = y;
                        //currDIST = "right";
                        if (x > 0)
                            wantedDIST = "left";
                        else
                            wantedDIST = "right";
                        found = true;
                    }
                }
            
            //Console.WriteLine(currDIST);
            if(stop!=true)
              Move_Logic(currDIST, wantedDIST,x,y);
        }

        //seting client uniqe ID and declare server and clients
        public ClientObject(TcpClient tcpClient, ServerObject serverObject)
        {
            Id = Guid.NewGuid().ToString();
            client = tcpClient;
            server = serverObject;
            serverObject.AddConnection(this);
            lives = 20;
            AddKeys();
          
        }
         
        //Main func
        public void Process()
        {
          

            try
            {
                client.SendTimeout = 1000;
                client.ReceiveTimeout = 1000;
                string tmp = null;
                Stream = client.GetStream();
                string message = null;

 
                
                while (true)
                {
                    
                    try
                    {

                        if (splitted.Count != 0 && splitedCount >= splitted.Count)
                            long_mess = false;
                        //If pos is [0,0] getting message
                        if (pickup)
                        {
                            server.SentMessage(SERVER_PICK_UP, this.Id);
                            do
                            {

                                message = GetMessage(Stream);
                                if (!check_len(message + "\a\b", 5))
                                    break;
                                tmp += message;
                                message = tmp;
                            }
                            while (!message.EndsWith("\a\b"));
                            tmp = null;
                            if (message.Equals("RECHARGING\a\b"))
                            {
                                server.SentMessage(SERVER_LOGIC_ERROR, this.Id);
                                break;
                            }

                            if (!check_len(message, 5) || !check_Syntax(message, 5, this.Id, Stream))
                            {
                                server.SentMessage(SERVER_SYNTAX_ERROR, this.Id);
                                break;
                            }

                            server.SentMessage(SERVER_LOGOUT, this.Id);
                            break;
                        }

                        if (Needcharged)
                        {
                            server.SentMessage(SERVER_LOGIC_ERROR, this.Id);
                            break;
                        }
                        //Getting mess
                        if (!long_mess)
                        {
                            do
                            {

                                message = GetMessage(Stream);
                                if (!check_len(message+"\a\b", stage))
                                    break;
                                tmp += message;
                               
                                message = tmp;
                               
                               
                            }
                            while (!message.EndsWith("\a\b"));
                            
                            tmp = null;
                            long_mess = Regex.Matches(message, "\a\b").Count > 1;
                        }
                        if (long_mess)
                        {
                            if (splitedCount == 0)
                                SplitThemOut(message);
                            message = splitted[splitedCount];
                            splitedCount++;
                        }

                        //Recharging logic
                        if (message.Equals("RECHARGING\a\b"))
                        {

                            Needcharged = true;
                            client.SendTimeout = 5000;
                            client.ReceiveTimeout = 5000;
                            do
                            {

                                message = GetMessage(Stream);
                                tmp += message;
                                message = tmp;
                            }
                            while (!message.EndsWith("\a\b"));
                            tmp = null;

                            if (message.Equals("FULL POWER\a\b"))
                            {
                                Needcharged = false;
                                client.SendTimeout = 1000;
                                client.ReceiveTimeout = 1000;
                                do
                                {

                                    message = GetMessage(Stream);
                                    tmp += message;
                                    message = tmp;
                                }
                                while (!message.EndsWith("\a\b"));
                                tmp = null;
                               
                            }
                            else
                            {

                                server.SentMessage(SERVER_LOGIC_ERROR, this.Id);
                                throw new Exception("SERVER_SYNTAX_ERROR");
                                
                            }
                        }
                       
                        //STAGE 0 - GETING USERNAME
                        if (stage == 0)
                        {
                            if (check_len(message, stage) && check_Syntax(message, stage, this.Id, Stream))
                            {
                                userName = message;
                                stage++;
                                server.SentMessage(SERVER_KEY_REQUEST, this.Id);

                            }
                            else
                            {
                                server.SentMessage(SERVER_SYNTAX_ERROR, this.Id);
                                throw new Exception("SERVER_SYNTAX_ERROR");
                            }

                        }

                        //STAGE 1 - GETING ID_KEY
                        else if (stage == 1)
                        {
                            if (check_len(message, stage) && check_Syntax(message, stage, this.Id, Stream))
                            {
                                if (check_Key(message))
                                {
                                    Key1 = message;
                                    Find_Key_hash(keys, this.Id);
                                    stage++;
                                }
                                else
                                {
                                    server.SentMessage(SERVER_KEY_OUT_OF_RANGE_ERROR, this.Id);
                                    throw new Exception("SERVER_KEY_OUT_OF_RANGE_ERROR");
                                }
                            }
                            else
                            {
                                server.SentMessage(SERVER_SYNTAX_ERROR, this.Id);
                                throw new Exception("SERVER_SYNTAX_ERROR1");
                            }
                        }
                        //STAGE 2 - CHECKING HASHES
                        else if (stage == 2)
                        {
                            if (check_len(message, stage) && check_Syntax(message, stage, this.Id, Stream))
                            {
                                klientHash = pure_datas(message);
                                if (Convert.ToString(GetHash(Key1, ClientKey, ClientKey, userHash)).Equals(klientHash))
                                {
                                    server.SentMessage(SERVER_OK, this.Id);
                                    server.SentMessage(SERVER_MOVE, this.Id);
                                    stage++;
                                    wasMove = true;

                                }
                                else
                                {
                                    server.SentMessage(SERVER_LOGIN_FAILED, this.Id);
                                    throw new Exception("SERVER_LOGIN_FAILED");

                                }

                            }
                            else
                            {
                                server.SentMessage(SERVER_SYNTAX_ERROR, this.Id);
                                throw new Exception("SERVER_SYNTAX_ERROR2");
                            }
                        }
                        // STAGE 4 - MOVING ROBOT and Geting Position/
                        else if (message.StartsWith("OK ")&&check_len(message,3) && check_Syntax(message, stage, this.Id, Stream))
                        {         
                                getPosition( pure_position(message),this.Id);                          
                                Move(posx, posy);                           
                        }

                        else
                        {
                                server.SentMessage(SERVER_SYNTAX_ERROR, this.Id);
                                throw new Exception("SERVER_SYNTAX_ERROR4");
                        }
                       
                    }

                    catch
                    {             
                        break;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                Console.WriteLine(lives);
                string status = String.Format("{0}: Failed", userName);
                Console.WriteLine(status);
                server.RemoveConnection(this.Id);
                Close();
                        
            }
        }
    
        protected internal void Close()
        {
            if (Stream != null)
                Stream.Close();
            if (client != null)
                client.Close();
        }
    }

    //Server funcs
    public class ServerObject
    {
        static TcpListener tcpListener; 
        List<ClientObject> clients = new List<ClientObject>(); 
        
        protected internal void AddConnection(ClientObject clientObject)
        {
            clients.Add(clientObject);
        }
        protected internal void RemoveConnection(string id)
        {
          
            ClientObject client = clients.FirstOrDefault(c => c.Id == id);
            
            if (client != null)
                clients.Remove(client);
        }
        
        protected internal void Listen()
        {
            try
            {
               tcpListener = new TcpListener(IPAddress.Any, 8888);
                tcpListener.Start();
               

               
                Console.WriteLine("Server was started. Waiting for connection...");
                
                while (true)
                {
                    

                    TcpClient tcpClient = tcpListener.AcceptTcpClient();
                    ClientObject clientObject = new ClientObject(tcpClient, this);


                    Thread clientThread = new Thread(new ThreadStart(clientObject.Process));
                    clientThread.Start();
                   
                   
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                Disconnect();
            }
        }

       
        protected internal void SentMessage(string message, string id)
        {
            byte[] data = Encoding.UTF8.GetBytes(message);
            for (int i = 0; i < clients.Count; i++)
            {
                if (clients[i].Id == id)
                {
                    clients[i].Stream.Write(data, 0, data.Length);
                }
            }
        }
       
        protected internal void Disconnect()
        {
            tcpListener.Stop();

            for (int i = 0; i < clients.Count; i++)
            {
                clients[i].Close();
            }
            Environment.Exit(0); 
        }
    }
    class Program
    {
        static ServerObject server; 
        static Thread listenThread; 
        static void Main(string[] args)
        {
            try
            {
                server = new ServerObject();
                
                listenThread = new Thread(new ThreadStart(server.Listen));
                listenThread.Start();
            }
            catch (Exception ex)
            {
                server.Disconnect();
                Console.WriteLine(ex.Message);
            }
        }
    }
}
