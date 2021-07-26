//using System.Collections;
//using System.Collections.Generic;
using UnityEngine;
using WebSocketSharp;
using UnityEngine.UI;

public class SocketDataClient : MonoBehaviour
{
    // Start is called before the first frame update
    WebSocket ws;
    public Vector3[] wristBands = new Vector3[5];


    //public Text dataText;
    //public int currentScore;

    //private int score = 1;
    public int bandX;
    public int bandY;
    public int bandZ;
    private CubeManager cubeManager;

    void Awake()
    {
        cubeManager = GameObject.FindObjectOfType<CubeManager>();
    }


    // Start is called before the first frame update
    void Start()
    {
        wristBands[0] = new Vector3(0.0f, 0.0f, 0.0f);
        wristBands[1] = new Vector3(0.0f, 0.0f, 0.0f);
        wristBands[2] = new Vector3(0.0f, 0.0f, 0.0f);
        wristBands[3] = new Vector3(0.0f, 0.0f, 0.0f);
        wristBands[4] = new Vector3(0.0f, 0.0f, 0.0f);

        Debug.Log("starting socket server");

        // Place Glitch Link
        ws = new WebSocket("wss://humorous-tasty-age.glitch.me");
        //ws = new WebSocket("wss://four-cube-parser-mac-id.glitch.me");
        ws.Connect();
        ws.OnMessage += (sender, e) =>
        {

            // when a new wristBand Arrives -- its ID will = length of the unity array.
            // in this case we have to add to array (how ?) and create a vector in the array

            Debug.Log("Message Received from " + ((WebSocket)sender).Url + ", Data : " + e.Data);
            string incomingMessage = e.Data;

            string[] elements = incomingMessage.Split(',');

            int id = int.Parse(elements[0]);
            Debug.Log("id " + id);

            foreach (var element in elements)
            {
                Debug.Log(element);
            }

            wristBands[id].Set(float.Parse(elements[1]), float.Parse(elements[2]), float.Parse(elements[3]));

            Debug.Log("wrist band data x " + wristBands[id].x);
            Debug.Log("wrist band data y" + wristBands[id].y);
            Debug.Log("wrist band data z " + wristBands[id].z);

            int bandX = (int)wristBands[id].x;
            int bandY = (int)wristBands[id].y;
            int bandZ = (int)wristBands[id].z;
        };
    }

    // Update is called once per frame
    void Update()
    {
        //cubeManager.UpdateData(score);
        cubeManager.UpdateData(bandX, bandY, bandZ);
        //cubeManager.UpdateData(bandY);
        //cubeManager.UpdateData(bandZ);
        if (ws == null)
        {
            Debug.Log("ws fail in unity");
            return;
        }
        if (Input.GetKeyDown(KeyCode.Space))
        {
            Debug.Log("space pressed");
            ws.Send("Hello");
        }
    }

    //void OnCollisionEnter2D(Collision2D collision)
    //{
    //    cubeManager.UpdateData(score);
    //}

    //public void UpdateData(int score)
    //{
    //    currentScore += score++;
    //    dataText.text = "Data: " + currentScore.ToString();
    //}
}
