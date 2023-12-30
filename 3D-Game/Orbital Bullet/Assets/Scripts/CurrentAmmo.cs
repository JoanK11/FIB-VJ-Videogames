using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
public class CurrentAmmo : MonoBehaviour
{
    // Start is called before the first frame update
    TMP_Text ammoText;
    void Start()
    {
        ammoText = gameObject.GetComponentInChildren<TMP_Text>();
    }
    public void SetAmmo(int ammo) {
        ammoText.text = ammo.ToString();
    }
    // Update is called once per frame
    
}
