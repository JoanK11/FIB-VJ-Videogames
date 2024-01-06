using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponNormal : WeaponBase {
    public GameObject prefab;
    
    public override void Shoot(Vector3 pos, Quaternion initialRotation, Transform parent, float orientation) {
        GameObject newObject = Instantiate(prefab, pos, initialRotation, parent);
        BalaPlayer bEnemy = newObject.AddComponent<BalaPlayer>();
        Debug.Log("orientation: " + orientation);
        bEnemy.init();
        bEnemy.setOrientation(orientation);
    }
}
