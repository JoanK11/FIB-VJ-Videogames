using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponLong : WeaponBase
{
    // Start is called before the first frame update
    public GameObject prefab;
    const int maxAmmo = 50;
    void Start() { 
        base.Start();
        ammo = maxAmmo;
    }
    public override void Shoot(Vector3 pos, Quaternion initialRotation, Transform parent, float orientation)
    {
        if (ammo > 0) {
            GameObject newObject = Instantiate(prefab, pos, initialRotation * prefab.transform.rotation, parent);

            BalaLongPlayer bEnemy = newObject.AddComponent<BalaLongPlayer>();
            Debug.Log("orientation: " + orientation);
            bEnemy.init();
            bEnemy.setOrientation(orientation);
            --ammo;
        }
        
    }
}
