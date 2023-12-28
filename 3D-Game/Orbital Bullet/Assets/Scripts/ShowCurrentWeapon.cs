using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class ShowCurrentWeapon : MonoBehaviour
{
    Image[] images;
    GameObject[] weapons;
    // Start is called before the first frame update
    void Start()
    {
        weapons = new GameObject[2];
        weapons[0] = GameObject.Find("Bow1");
        weapons[1] = GameObject.Find("Bow2");
        images = GetComponentsInChildren<Image>();
        foreach (Image image in images) {
            image.enabled = false;
        }
    }

    // Update is called once per frame
    void Update()
    {
        foreach (Image image in images)
        {
            image.enabled = false;
        }
        if (weapons[0].activeSelf)
        {
            images[0].enabled = true;
        }
        else {
            images[1].enabled = true;
        }
    }
}
