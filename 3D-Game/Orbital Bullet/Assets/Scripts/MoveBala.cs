using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveBala : MonoBehaviour
{
    // Start is called before the first frame update

    public float rotationSpeed, jumpSpeed, gravity;

    Vector3 startDirection;
    
    Quaternion rotacionInicial;

    void Start()
    {
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        rotacionInicial = transform.rotation;
       
    }

    // Update is called once per frame
    void FixedUpdate()
    {
     //   Debug.Log("entro en el update");
        
        Vector3 position;
        float angle;
        Vector3 direction, target;

        position = transform.position;
        angle = rotationSpeed * Time.deltaTime;
        direction = position - transform.parent.position;

        target = transform.parent.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;
        
        transform.position = target;
        Physics.SyncTransforms();
      

        // Correct orientation of player
        // Compute current direction
        
        Vector3 currentDirection = transform.position - transform.parent.position;
        currentDirection.y = 0.0f;
        currentDirection.Normalize();
        // Change orientation of player accordingly
        Quaternion orientation;
        if ((startDirection - currentDirection).magnitude < 1e-3)
            orientation = Quaternion.AngleAxis(0.0f, Vector3.up);
        else if ((startDirection + currentDirection).magnitude < 1e-3)
            orientation = Quaternion.AngleAxis(180.0f, Vector3.up);
        else
            orientation = Quaternion.FromToRotation(startDirection, currentDirection);
        transform.rotation = orientation * rotacionInicial;
        

    }
    private void OnTriggerEnter(Collider other)
    {
        Debug.Log(other.gameObject.name + " ha entrado en el colider de " + gameObject.name);
        MeshRenderer mesh = GetComponent<MeshRenderer>();
        mesh.enabled = false;

        if (other.gameObject.tag == "Enemy") {
            MoveEnemy1 enemy = other.gameObject.GetComponent<MoveEnemy1>();
            enemy.takeDamage(10);
        }
        gameObject.SetActive(false);
        Destroy(this);
    }
}
