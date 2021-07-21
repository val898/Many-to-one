// Example reterived from https://www.youtube.com/playlist?list=PLbghT7MmckI4Za8nEwpwzKsK3V0Tf53Tp
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using SimpleJSON;

public class ServerTalker : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        // Make a web request to get info from the server
        // this wil be a text response.
        // This will return/contune IMMEDIATELY, but the coroutine
        // will take several MS to actually get a response from the server.
        StartCoroutine(GetWebData("http://localhost:8000/user/", "myAwesomeID"));
    }


    void ProcessSeverResponse(string rawResponse)
    {
        // That text, is actually JSON info, so we need to
        // parse that into something we can navigate.

        JSONNode node = JSON.Parse(rawResponse);

        // Output some stuff to the console so that we know
        // that it worked.

        Debug.Log("Username: " + node["username"]);
        Debug.Log("Misc Data: " + node["someArray"][1]["name"] + " = " + node["someArray"][1]["value"]);

        //PlayerData.SetBar(node["someArray"][1]["value"]);
    }

    IEnumerator GetWebData(string address, string myID)
    {
        UnityWebRequest www = UnityWebRequest.Get(address + myID);
        //UnityWebRequest www = UnityWebRequest.Post();
        yield return www.SendWebRequest();

        if (www.result != UnityWebRequest.Result.Success)
        {
            Debug.LogError("Something went wrong: " + www.error);
        }
        else
        {
            Debug.Log(www.downloadHandler.text);

            ProcessSeverResponse(www.downloadHandler.text);
        }
    }


    // Update is called once per frame
    void Update()
    {
        
    }
}
