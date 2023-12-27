using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponLong : WeaponBase
{
    // Start is called before the first frame update
    public GameObject prefab;
    public override void Shoot(Vector3 pos, Quaternion initialRotation, Transform parent, float orientation)
    {
        GameObject newObject = Instantiate(prefab, pos, initialRotation * prefab.transform.rotation, parent);

        BalaLongPlayer bEnemy = newObject.AddComponent<BalaLongPlayer>();
        Debug.Log("orientation: " + orientation);
        bEnemy.init();
        bEnemy.setOrientation(orientation);
    }
}
