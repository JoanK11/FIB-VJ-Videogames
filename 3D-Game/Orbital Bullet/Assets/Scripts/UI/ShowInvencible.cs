using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ShowInvencible : MonoBehaviour
{
    // Start is called before the first frame update
    public MovePlayer player;
    Image img;
    void Start()
    {
        img = GetComponentInChildren<Image>();
    }

    // Update is called once per frame
    void Update()
    {
        img.enabled = player.IsInvencible();
    }
}
