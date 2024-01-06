using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponLong : WeaponBase {
    public GameObject prefab;
   
    public override void Shoot(Vector3 pos, Quaternion initialRotation, Transform parent, float orientation) {
        GameObject newObject = Instantiate(prefab, pos, initialRotation, parent);

        BalaLongPlayer bEnemy = newObject.AddComponent<BalaLongPlayer>();
        Debug.Log("orientation: " + orientation);
        bEnemy.init();
        bEnemy.setOrientation(orientation);
    }
}
