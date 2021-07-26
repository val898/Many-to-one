using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CubeScript : MonoBehaviour
{

    public Vector3 spinSpeed = Vector3.zero;

    public Vector3 spinAxis = new Vector3(0, 1, 0);

    public float rotateSpeed = 1.0f;

    // Start is called before the first frame update
    void Start()
    {
        //SetSize(2.0f);
        //this.transform.position = new Vector3(0, 5, 3);
        spinSpeed = new Vector3(Random.value, Random.value, Random.value);

        spinAxis = Vector3.up;
        spinAxis.x = (Random.value - Random.value) * .1f; //0 - 1 = -1 (Lowest value possible) //1 - 0 = +1 (Highest value possible)
    }


    public void SetSize(float size)
    {
        this.transform.localScale = new Vector3(size, size, size);
    }


    // Update is called once per frame
    void Update()
    {
        this.transform.Rotate(spinSpeed);
        this.transform.RotateAround(Vector3.zero, spinAxis, rotateSpeed);


    }
}
