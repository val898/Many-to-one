using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sketch : MonoBehaviour
{

    public GameObject myPrefab;

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
            float sin = Mathf.Sin(perc * Mathf.PI/2);

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

    }
}
