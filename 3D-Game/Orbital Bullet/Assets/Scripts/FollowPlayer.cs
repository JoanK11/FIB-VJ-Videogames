using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowPlayer : MonoBehaviour
{
    public GameObject player;
    float incry;

    // Start is called before the first frame update
    void Start()
    {
        incry = transform.position.y - player.transform.position.y;
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 P = transform.position;
        P.y = player.transform.position.y + incry;
        transform.position = P;
    }
}
