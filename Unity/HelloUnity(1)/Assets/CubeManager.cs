using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CubeManager : MonoBehaviour
{

    public GameObject myPrefab;

    public Text dataText;
    public int currentScore;

    //public GameObject cubes;


    //private SocketDataClient sockets;
    //private int score = 1;

    //void Awake()
    //{
    //    sockets = GameObject.FindObjectOfType<SocketDataClient>();
    //}


    // Start is called before the first frame update
    void Start()
    {
        int totalCubes = 30;

        float totalDistance = 2.9f;

        ////LINEAR DISTROBUTION
        //for (int i = 0; i < totalCubes; i++)
        //{
        //    float perc = i / (float)totalCubes;

        //    float x = perc * totalDistance;
        //    float y = 3.0f;
        //    float z = 0.0f;

        //    var newCube = (GameObject)Instantiate(myPrefab, new Vector3(x, y, z), Quaternion.identity);
        //    newCube.GetComponent<CubeScript>().SetSize(1.0f - perc);
        //    newCube.GetComponent<CubeScript>().rotateSpeed = 0; // perc; // Random.value;
        //}

        //SIN() DISTROBUTION
        for (int i = 0; i < totalCubes; i++)
        {
            float perc = i / (float)totalCubes;
            float sin = Mathf.Sin(perc * Mathf.PI / 2);

            float x = 1.8f + (sin * totalDistance);
            float y = 5.0f;
            float z = 0.0f;

            var newCube = (GameObject)Instantiate(myPrefab, new Vector3(x, y, z), Quaternion.identity);

            newCube.GetComponent<CubeScript>().SetSize(.45f * (1.0f - perc));
            newCube.GetComponent<CubeScript>().rotateSpeed = .2f + (perc * 4.0f); //  0; // perc; // Random.value;
        }

    }

    // Update is called once per frame
    void Update()
    {
        //sockets.UpdateData(score);
    }

    //public void UpdateData(int score)
    //{
    //    currentScore += score++;
    //    dataText.text = "Data: " + currentScore.ToString();
    //}

    public void UpdateData(int bandX, int bandY, int bandZ)
    {
        dataText.text = "AccX: " + bandX.ToString() + ", AccY: " + bandY.ToString() + ", AccZ: " + bandZ.ToString();
    }

    //private void OnCollisionEnter2D(Collision2D collision)
    //{
    //    sockets.UpdateData(score);
    //}
}
