using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class WeaponBase : MonoBehaviour {
    public abstract void Shoot(Vector3 pos, Quaternion initialRotation, Transform parent, float orientation);
}
