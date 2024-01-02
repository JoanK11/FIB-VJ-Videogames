using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class WeaponBase : MonoBehaviour
{
    // Start is called before the first frame update

    protected int ammo;
    CurrentAmmo text;
    public abstract void Shoot(Vector3 pos, Quaternion initialRotation, Transform parent, float orientation);
    protected void Start() {
        text = GameObject.Find("CurrentAmmo").GetComponent<CurrentAmmo>();
    }
    void Update() {
        if (gameObject.activeSelf)
        {
            text.SetAmmo(ammo);
        }
        
    }
}
